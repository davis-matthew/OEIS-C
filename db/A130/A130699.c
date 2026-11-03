/*
 * A130699  Numbers n for which neither 2n-3 nor 2n+3 are primes.
 * https://oeis.org/A130699
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130699: compute the n-th integer n such that neither 2n-3 nor 2n+3 are primes.
   The sequence starts with a(2)=6, a(3)=9, … (offset 2). This code builds a list of
   such numbers on demand and returns the requested term. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2; values below the offset are undefined. */
    if (n < 2)
        return (seq_t)0; /* arbitrary for out-of-range indices */

    static seq_t *list = NULL;       /* list of terms (a(2), a(3), …) */
    static size_t count = 0;         /* number of terms stored */
    static size_t capacity = 0;      /* allocated capacity for list */
    static idx_t last_candidate = 2; /* next candidate value to examine */

    /* Generate terms until we have at least n terms. */
    while (count < n)
    {
        /* Check primes 2*last_candidate-3 and 2*last_candidate+3. */
        idx_t val1 = 2 * last_candidate - 3;
        idx_t val2 = 2 * last_candidate + 3;
        bool p1 = is_prime((idx_t)(val1 >= 2 ? val1 : 0));
        bool p2 = is_prime((idx_t)(val2 >= 2 ? val2 : 0));

        if (!p1 && !p2)
        {
            /* Need space for an additional element. */
            grow_seq(&list, &capacity, count + 1);
            list[count++] = (seq_t)last_candidate;
        }
        ++last_candidate;
    }

    /* The first stored element corresponds to n = 2, so adjust index. */
    return list[n - 2];
}
