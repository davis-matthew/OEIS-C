/*
 * A102218  2-Monica numbers.
 * https://oeis.org/A102218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_parity, is_composite
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A102218
   Definition: Composite numbers k such that the difference between
   the sum of digits of k and the sum of sums of digits of the prime
   factors of k (with multiplicity) is even, i.e. the parities match.
   This implementation finds the nth such number by testing integers
   sequentially. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum_parity.h"
#include "../utils/is_composite.h"

/* dynamic storage for the sequence */
static seq_t *seq_vals = NULL;
static size_t seq_cap = 0;
static idx_t seq_cnt = 0;
static idx_t next_candidate = 4; /* start scanning from 4 */

/* ensure capacity for at least req elements */
static void ensure_capacity(size_t req)
{
    if (req <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < req)
        new_cap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined, but safe */

    /* extend sequence until it contains at least n terms */
    while (seq_cnt < n) {
        uint64_t k = (uint64_t)next_candidate;
        ++next_candidate;

        if (!is_composite(k))
            continue;           /* skip primes */

        /* parity of digit sum of k */
        int parity_k = digit_sum_parity(k);

        /* parity of sum of digit sums of prime factors (with multiplicity) */
        int parity_factors = 0;
        uint64_t temp = k;
        for (uint64_t p = 2; p * p <= temp; ++p) {
            while (temp % p == 0) {
                parity_factors ^= digit_sum_parity(p);
                temp /= p;
            }
        }
        if (temp > 1)             /* remaining prime factor */
            parity_factors ^= digit_sum_parity(temp);

        if (parity_k == parity_factors) {
            ensure_capacity(seq_cnt + 1);
            seq_vals[seq_cnt++] = (seq_t)k;
        }
    }

    return seq_vals[n - 1];      /* 0‑based index */
}
