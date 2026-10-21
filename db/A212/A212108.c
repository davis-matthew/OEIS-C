/*
 * A212108  Number of (w,x,y,z) with all terms in {1,...,n} and w*x-y*z<n.
 * https://oeis.org/A212108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (w,x,y,z) with 1 ≤ w,x,y,z ≤ n and w*x – y*z <
 *            n Computation details: Count all 4‑tuples and subtract those
 *            with w*x – y*z ≥ n. The complement can be expressed using the
 *            distribution of products of two numbers in [1,n]. A[p] = #{(u,v)
 *            | 1≤u,v≤n, u*v = p}. For each product p, all...
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) {
        return 0;
    }

    /* Compute size of product count array: max product = n * n. */
    size_t max_prod = (size_t)n * (size_t)n;
    size_t array_size = max_prod + 1;

    /* Allocate array of 16‑bit counters for A[p] */
    uint16_t *A = (uint16_t *)calloc(array_size, sizeof(uint16_t));
    if (!A) {
        return 0; /* Allocation failure; return 0 as fallback. */
    }

    /* 1. Build distribution of products of two numbers in [1,n] */
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            size_t prod = (size_t)w * (size_t)x;
            ++A[prod];
        }
    }

    /* 2. Compute complement sum using a circular buffer for prefix sums */
    size_t buff_size = (size_t)n + 1;
    seq_t *pref_buf = (seq_t *)calloc(buff_size, sizeof(seq_t));
    if (!pref_buf) {
        free(A);
        return 0;
    }
    seq_t cum = 0;          /* cumulative sum of A up to current index - 1 */
    seq_t complement = 0;   /* sum of A[p] * prefixA[p-n] */

    for (size_t p = 0; p <= max_prod; ++p) {
        uint16_t cnt = A[p];
        seq_t prefix_before = 0;
        if (p >= (size_t)n) {
            size_t idx = (p - (size_t)n) % buff_size;
            prefix_before = pref_buf[idx];
        }
        complement += (seq_t)cnt * prefix_before;
        cum += (seq_t)cnt;
        pref_buf[p % buff_size] = cum;
    }

    free(A);
    free(pref_buf);

    /* 3. Total 4‑tuples = n^4, compute n^4 safely */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;     /* n^2 */
    seq_t n4 = n2 * n2;     /* n^4 */

    /* 4. Return required count */
    return n4 - complement;
}
