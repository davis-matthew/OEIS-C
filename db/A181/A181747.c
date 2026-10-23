/*
 * A181747  Odd noncomposite numbers that are the difference of 2 primes.
 * https://oeis.org/A181747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181747: odd non‑composite numbers that are the difference of two primes.
 * These are 1 and the lower members of all twin prime pairs.
 *
 * a(1) = 1
 * for n >= 2, a(n) is the (n-2)th lower twin prime (starting from 3).
 *
 * The implementation builds the list of lower twin primes incrementally
 * using the supplied `is_prime` routine.  The list is memoised so that
 * calls in any order are efficient.  All arithmetic is performed in
 * idx_t to avoid overflow; results are converted to seq_t for return.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* declares: bool is_prime(idx_t); */

/* static cache of lower twin primes (values stored as seq_t) */
static seq_t *lower = NULL;          /* array of lower twin primes */
static size_t lower_sz = 0;          /* number of primes currently stored */
static size_t lower_cap = 0;         /* current allocation capacity */

/* next number to test for primality (always odd >= 3) */
static idx_t next_check = 3;

/* Ensure that the dynamic array has at least `new_cap` elements. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= lower_cap)
        return;
    size_t newsize = (lower_cap == 0 ? 1 : lower_cap * 2);
    while (newsize < new_cap)
        newsize <<= 1;               /* double until large enough */
    seq_t *tmp = (seq_t *)realloc(lower, newsize * sizeof(seq_t));
    if (!tmp) {
        /* Out of memory – abort (sequence generation cannot continue) */
        exit(EXIT_FAILURE);
    }
    lower = tmp;
    lower_cap = newsize;
}

/* Append a new lower twin prime value to the cache. */
static void add_lower(idx_t val)
{
    if (lower_sz >= lower_cap)
        ensure_capacity(lower_sz + 1);
    lower[lower_sz++] = (seq_t)val;
}

/* Grow the cache until it contains at least `need` lower twin primes. */
static void ensure_lower_size(size_t need)
{
    while (lower_sz < need) {
        /* Find the next odd prime >= next_check. */
        while (!is_prime(next_check))
            ++next_check;

        /* If the next prime has a twin (p+2 is prime), store it. */
        if (is_prime(next_check + 2))
            add_lower(next_check);

        /* Move to the next odd number for the next iteration. */
        next_check += 2;
    }
}

/* ------------------------------------------------------------------------- */
/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;           /* the first special term */

    /* For n >= 2, we need (n-1) lower twin primes. */
    size_t need = (size_t)(n - 1);
    ensure_lower_size(need);

    return lower[need - 1];        /* zero‑based index of the requested term */
}
