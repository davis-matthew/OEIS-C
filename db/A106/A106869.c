/*
 * A106869  Primes of the form x^2+xy+6y^2, with x and y nonnegative.
 * https://oeis.org/A106869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form x^2 + x*y + 6*y^2 with x,y ≥ 0
 * Range:     u8 none, u16 n <= 248, u32 n <= 916, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq, isprime_big, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility headers for the helper functions.  The build system
   supplies these and declares the prototypes. */
#include "../utils/isprime_big.h"   /* bool isprime_big(seq_t) */
#include "../utils/cmp_seq.h"      /* int cmp_seq(const void*, const void*) */
#include "../utils/push_seq.h"     /* void push_seq(seq_t **, idx_t *, idx_t *, seq_t) */

/* Static state holding the enumeration progress. */
static seq_t *prime_form_primes = NULL;   /* array of found primes */
static idx_t   prime_form_cap   = 0;      /* current capacity of the array */
static idx_t   prime_form_len   = 0;      /* number of primes stored */
static idx_t   enumeration_limit = 0;     /* maximum value already processed */

/* Append a primitive prime to the dynamic array. */
static void add_prime(seq_t val) {
    push_seq(&prime_form_primes, &prime_form_cap, &prime_form_len, val);
}

/* Generate all values of the quadratic form in the range
   (old_limit, new_limit] and test them for primality.  New primes are
   appended to the static array. */
static void generate_new_primes(idx_t old_limit, idx_t new_limit) {
    for (idx_t y = 0; /* no init */; ++y) {
        seq_t y2   = (seq_t)y * y;
        seq_t base = (seq_t)6 * y2;               /* value when x = 0 */
        if (base > (seq_t)new_limit)
            break;                                /* further y produce only larger values */

        for (idx_t x = 0; /* no init */; ++x) {
            seq_t val = (seq_t)x * x + (seq_t)x * y + base;
            if (val > (seq_t)new_limit)
                break;                            /* for this y, larger x only increase val */

            if (val > (seq_t)old_limit && val >= (seq_t)2 && isprime_big(val))
                add_prime(val);
        }
    }
}

/* Sort the array of primes and remove duplicates.  This is called
   after a new batch of primes has been appended. */
static void sort_and_dedup_primes(void) {
    if (prime_form_len == 0)
        return;

    qsort(prime_form_primes, prime_form_len, sizeof(seq_t), cmp_seq);

    /* Remove duplicates in-place. */
    idx_t j = 0;
    for (idx_t i = 0; i < prime_form_len; ++i) {
        if (j == 0 || prime_form_primes[i] != prime_form_primes[j - 1])
            prime_form_primes[j++] = prime_form_primes[i];
    }
    prime_form_len = j;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)               /* the OEIS sequence is defined for n ≥ 1 */
        return 0;

    /* Grow the search range until we have at least n primes. */
    while (prime_form_len < n) {
        idx_t new_limit = (enumeration_limit == 0) ? 1000 : enumeration_limit * 2;
        generate_new_primes(enumeration_limit, new_limit);
        enumeration_limit = new_limit;
        sort_and_dedup_primes();
    }

    return prime_form_primes[n - 1];   /* zero‑based array index */
}
