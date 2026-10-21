/*
 * A226349  Number of functions f:{1,2,...,n} -> {1,2,...,n} such that the 1
 *          and the 2 are in the same component of the functional digraph
 *          representation of f.
 * https://oeis.org/A226349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=2}^{n} C(n‑2, k‑2) · c_k · (n‑k)^{n‑k} where c_k is
 *            the number of connected endofunctions on k labeled nodes,
 *            computed by c_1 = 1 and for k ≥ 2 c_k = k^k – ∑_{t=1}^{k‑1}
 *            C(k‑1, t‑1) · c_t · (k‑t)^{k‑t}
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* cached arrays for powers and connected counts */
static seq_t *pow_arr = NULL;
static seq_t *c_arr  = NULL;
static size_t cached_n = 0;

/* precompute values up to N (inclusive) */
static void precompute(idx_t N)
{
    if (N <= cached_n) return;

    size_t new_size = N + 1;
    pow_arr = (seq_t *)realloc(pow_arr, new_size * sizeof(seq_t));
    c_arr  = (seq_t *)realloc(c_arr,  new_size * sizeof(seq_t));

    /* ensure base cases */
    pow_arr[0] = (seq_t)1;
    if (N >= 1) {
        c_arr[1] = (seq_t)1;
    }

    /* compute from last cached+1 up to N */
    for (idx_t i = (idx_t)(cached_n + 1); i <= N; ++i) {
        /* i^i */
        pow_arr[i] = big_pow((seq_t)i, (seq_t)i);

        if (i == 1) {            /* base case already set */
            c_arr[1] = (seq_t)1;
            continue;
        }

        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;               /* C(i‑1, 0) */

        for (idx_t k = 1; k < i; ++k) {
            seq_t temp = binom;
            temp *= c_arr[k];
            temp *= pow_arr[i - k];
            sum += temp;

            if (k < i - 1) {
                binom = binom * (seq_t)(i - k); /* numerator */
                binom = binom / (seq_t)k;       /* denominator */
            }
        }
        c_arr[i] = pow_arr[i];
        c_arr[i] -= sum;
    }

    cached_n = N;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    precompute(n);

    seq_t result = (seq_t)0;
    seq_t binom  = (seq_t)1;              /* C(n‑2, 0) */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t temp = binom;
        temp *= c_arr[k];
        temp *= pow_arr[n - k];
        result += temp;

        if (k < n) {
            binom = binom * (seq_t)(n - k); /* numerator */
            binom = binom / (seq_t)(k - 1); /* denominator */
        }
    }

    return result;
}
