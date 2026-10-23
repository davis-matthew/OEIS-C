/*
 * A128111  Expansion of q^(-1) * (phi(q) / phi(q^9) - 1) / 2 in powers of q^3
 *          where phi() is a Ramanujan theta function.
 * https://oeis.org/A128111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 77, i32 n <= 251, i64 n <= 860,
 *            i128 n <= 1000, big any n
 * Helpers:   isqrt_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the coefficient of q^(3n) in 
 *      (q^(-1) * (phi(q)/phi(q^9) - 1)) / 2.
 * Let c[0] = 1 and for n>0
 *     c[n] = phi[n] - 2 * Σ_{m ≥ 1 , 9*m*m ≤ n} c[n - 9*m*m] ,
 * where phi[n] = 1 if n==0, 2 if n is a perfect square, otherwise 0.
 * Then a(n) = c[3*n + 1] / 2.  All divisions are exact.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt_small.h"

/* Dynamic array to store the sequence c[n] */
static seq_t *c_seq = NULL;   /* array of c values, indexed from 0 */
static idx_t c_max = 0;       /* current allocated size minus one */

/* Compute c up to desired index */
static void ensure_c(idx_t need)
{
    if (need <= c_max) return;

    size_t new_len = (size_t)(need + 1);
    seq_t *new_c = (seq_t *)realloc(c_seq, new_len * sizeof(seq_t));
    if (!new_c) exit(EXIT_FAILURE);                 /* out of memory */
    c_seq = new_c;

    /* Initialize missing entries */
    for (idx_t i = c_max + 1; i <= need; ++i) {
        /* phi[i] */
        seq_t phi_val;
        if (i == 0) {
            phi_val = 1;
        } else {
            idx_t r = isqrt_small(i);
            phi_val = (r * r == i) ? 2 : 0;
        }

        /* Σ 2 * c[i - 9*m^2] */
        seq_t sum = 0;
        for (idx_t m = 1; ; ++m) {
            idx_t offset = 9ULL * m * m;
            if (offset > i) break;
            sum += (seq_t)2 * c_seq[i - offset];
        }

        c_seq[i] = phi_val - sum;
    }

    c_max = need;
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    idx_t need_index = 3 * n + 1;
    ensure_c(need_index);
    seq_t val = c_seq[need_index];
    return val / 2;
}
