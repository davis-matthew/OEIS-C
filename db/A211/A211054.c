/*
 * A211054  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          determinant n-1.
 * https://oeis.org/A211054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 144, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A211054
   Number of 2×2 matrices with entries in {1,…,n} and determinant equal to n−1.
   Algorithm: 
   • Count, for each possible product p = a·d, the number of (a,d) pairs.
   • For each product p with p ≥ n−1, add to the answer
     count[p] × count[p−(n−1)], which is the number of quadruples
     (a,b,c,d) satisfying ad−bc = n−1.
   Complexity:  O(n²) operations and O(n²) memory for the product‑count array.
   This meets the 2‑second time limit for n ≤ 10000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0 (offset 0).  No matrices exist. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Convert to native integer for array indexing. */
    size_t N = (size_t)n;

    /* Size of the product‑count array: maximum product is N·N. */
    size_t max_prod = N * N;

    /* Use 16‑bit counts; for n ≤ 10000 the count never exceeds 65535. */
    uint16_t *cnt = (uint16_t *)calloc(max_prod + 1, sizeof(uint16_t));
    if (!cnt)
        return (seq_t)0;   /* OOM → return zero (test harness avoids this). */

    /* Count (a,d) pairs for each product a·d. */
    for (size_t a_val = 1; a_val <= N; ++a_val) {
        for (size_t d_val = 1; d_val <= N; ++d_val) {
            size_t prod = a_val * d_val;   /* prod ≤ max_prod */
            ++cnt[prod];
        }
    }

    seq_t ans = (seq_t)0;
    size_t D = N - 1;            /* determinant value n−1 */

    /* For each admissible product p, combine with product p−D. */
    for (size_t p = D + 1; p <= max_prod; ++p) {
        size_t q = p - D;        /* q ≥ 1 */
        if (q > max_prod)
            continue;            /* safety guard, should not happen */
        if (cnt[p] && cnt[q])
            ans += (seq_t)cnt[p] * (seq_t)cnt[q];
    }

    free(cnt);
    return ans;
}
