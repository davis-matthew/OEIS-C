/*
 * A121433  Number of subpartitions of partition
 *          P=[0,0,0,0,1,1,1,1,1,2,2,2,2,2,2,3,...], where P(n) =
 *          [(sqrt(8*n+49) - 7)/2].
 * https://oeis.org/A121433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 1 - Σ_{i=0}^{n-1} a(i) * C(P(i), n-i) *
 *            (-1)^{n-i} where P(i) = floor((sqrt(8*i+49)-7)/2) and C(P,i) is
 *            the binomial coefficient
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* Integer square‑root for unsigned 64‑bit integers. */
static idx_t _isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;          /* highest bit <= sqrt(x) */
    while (bit) {
        idx_t temp = r + bit;
        if (temp <= x / temp) {
            r = temp;
        }
        bit >>= 1;
    }
    return r;
}

/* Compute binomial coefficient C(N,K) for small N,K, using only seq_t operations. */
static seq_t binom_value(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i) {
        /* result = result * (N-K+i) / i  */
        result = (result * (seq_t)(N - K + i)) / (seq_t)i;
    }
    return result;
}

/* Main function computing the OEIS sequence A121433. */
seq_t a(idx_t n)
{
    /* Allocate an array for a[0..n] with proper construction for arbitrary seq_t types. */
    seq_t *a_val = seq_alloc_array(n + 1);
    idx_t *P_val = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!a_val || !P_val) {
        /* Memory allocation failed */
        return (seq_t)0;
    }

    for (idx_t i = 0; i <= n; ++i) {
        /* Compute P(i) = floor((sqrt(8*i+49)-7)/2) */
        idx_t val = 8 * i + 49;
        idx_t sqrt_val = _isqrt_u64(val);
        idx_t p = (sqrt_val >= 7) ? (sqrt_val - 7) / 2 : 0;
        P_val[i] = p;

        if (i == 0) {
            a_val[0] = 1;
            continue;
        }

        seq_t sum_even = 0, sum_odd = 0;
        for (idx_t j = 0; j < i; ++j) {
            idx_t k = i - j;
            if (k <= P_val[j]) {
                seq_t coeff = binom_value(P_val[j], k);
                seq_t prod  = a_val[j] * coeff;
                if (k & 1)
                    sum_odd += prod;
                else
                    sum_even += prod;
            }
        }
        a_val[i] = 1 + sum_odd - sum_even;
    }

    seq_t result = a_val[n];
    seq_free_array(a_val);
    free(P_val);
    return result;
}
