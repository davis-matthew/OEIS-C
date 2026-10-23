/*
 * A154608  Primes p such that 11*p + 4 is also prime.
 * https://oeis.org/A154608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 114, u32 n <= 115, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: the primes p such that 11*p + 4 is also prime (OEIS A154608). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

static seq_t *qual = NULL;      /* array of qualifying primes */
static size_t qual_cap = 0;     /* current capacity of the array */
static size_t qual_len = 0;     /* number of items stored so far */
static idx_t last_checked = 1;  /* the last number examined for the next prime */

/* Ensure that qual[] contains at least n qualifying primes. */
static void ensure_n(idx_t n)
{
    while (qual_len < (size_t)n) {
        /* Find the next prime after the last checked number. */
        uint64_t p = next_prime((uint64_t)last_checked);
        last_checked = (idx_t)p;

        /* Check whether 11*p + 4 is also prime. */
        seq_t m = (seq_t)p * (seq_t)11 + (seq_t)4;
        if (is_prime_big(m)) {
            /* Grow the array if necessary. */
            if (qual_len >= qual_cap) {
                size_t newcap = qual_cap ? (qual_cap << 1) : 64;
                seq_t *tmp = (seq_t *)realloc(qual, newcap * sizeof(seq_t));
                if (!tmp) abort();
                qual = tmp;
                qual_cap = newcap;
            }
            qual[qual_len++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* below offset, undefined behaviour */
    ensure_n(n);
    return qual[(size_t)(n - 1)];
}
