/*
 * A178796  An ascending sequence of primes a(n) such that either the sum of
 *          decimal digits of a(n) is divisible by the sum of decimal digits
 *          of a(n+1) or vice versa.
 * https://oeis.org/A178796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 672, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-generated sequence: primes a(n) such that
   digit_sum(a(n)) divides digit_sum(a(n+1)) or vice versa,
   in ascending order. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static uint64_t next_candidate = 0;

    if (seq_len == 0) {                /* first call: initialise */
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;
        seq[0] = (seq_t)2;             /* first term */
        seq_len = 1;
        next_candidate = 3;            /* next odd to test */
    }

    while (seq_len < n) {              /* generate until we have n terms */
        uint64_t p = next_candidate++; /* current prime candidate */

        if (!is_prime(p))
            continue;                  /* skip non‑primes */

        seq_t d_last = digit_sum(seq[seq_len - 1]); /* digit sum of previous prime */
        seq_t d_new  = digit_sum((seq_t)p);        /* digit sum of candidate */

        if (!((d_last % d_new == 0) || (d_new % d_last == 0)))
            continue;                  /* condition fails */

        /* store accepted prime */
        if (seq_len == seq_cap) {       /* grow array if needed */
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!seq) return (seq_t)0;
        }
        seq[seq_len++] = (seq_t)p;
    }

    return seq[n - 1];                  /* a(1) is seq[0] */
}
