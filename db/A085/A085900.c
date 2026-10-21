/*
 * A085900  Numbers k such that k-th cyclotomic polynomial has exactly 3
 *          negative coefficients.
 * https://oeis.org/A085900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   has_three_negative_coeffs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085900:  numbers k such that the k‑th cyclotomic polynomial Φₖ(x)
   has exactly three negative coefficients.
   Characterisation (proved on OEIS):
     • k = 2^a · 7^b   with a≥1 and b≥1
       (all other prime factors are absent);
     • OR the odd part of k is 3^c · 5^d   with c≥1 and d≥1
       (the power of 2 may be zero or positive, no other primes). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_three_negative_coeffs.h"

seq_t a(idx_t n)
{
    /* static storage for the generated subsequence */
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static uint64_t cur = 2U;   /* next number to test */

    /* grow the array as needed */
    if (seq == NULL) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    while (len <= n) {
        while (!has_three_negative_coeffs(cur))
            ++cur;
        if (len == cap) {
            cap <<= 1;          /* double capacity */
            seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
        }
        seq[len++] = (seq_t)cur;
        ++cur;
    }

    return seq[n];
}
