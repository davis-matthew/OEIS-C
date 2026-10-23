/*
 * A196557  O.g.f.: Sum_{n>=0} 4*(n+4)^(n-1) * x^n / Product_{k=1..n} (1+k*x).
 * https://oeis.org/A196557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * Stirling2(n,k) * 4 *
 *            (k+4)^(k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"

static seq_t *ensure_cap(seq_t *arr, idx_t *cap, idx_t need) {
    if (*cap <= need) {
        idx_t new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        arr = tmp;
        *cap = new_cap;
    }
    return arr;
}

static seq_t *st_cache = NULL;
static idx_t  st_cap   = 0;

seq_t a(idx_t n) {
    if (n == 0u) return (seq_t)1;

    /* Allocate and initialize Stirling array */
    st_cache = ensure_cap(st_cache, &st_cap, n);
    for (idx_t i = 0; i <= n; ++i) st_cache[i] = 0;
    st_cache[0] = 1;                /* S(0,0) = 1 */

    /* Build Stirling numbers of the second kind up to n */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t k = i; k >= 1; --k) {
            seq_t prod = seq_mul(st_cache[k], (seq_t)k);      /* k*S(i-1,k) */
            st_cache[k] = prod + st_cache[k - 1];            /* + S(i-1,k-1) */
        }
        st_cache[0] = 0; /* S(i,0) = 0 for i > 0 */
    }

    /* Compute the alternating sum */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t pow = big_pow((seq_t)(k + 4), (seq_t)(k - 1));   /* (k+4)^(k-1) */
        seq_t term = seq_mul(mul_small(pow, 4), st_cache[k]); /* 4 * pow * S(n,k) */
        if (((n - k) & 1u) == 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    /* sum_pos >= sum_neg because a(n) is non‑negative */
    seq_t result = sum_pos;
    result -= sum_neg;
    return result;
}
