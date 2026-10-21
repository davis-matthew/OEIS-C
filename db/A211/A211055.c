/*
 * A211055  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          determinant n+1.
 * https://oeis.org/A211055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 154, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A211055
    Count of 2 × 2 integer matrices with entries in {1,…,n} having determinant n+1.
    The routine builds the frequency of all products b·c (1 ≤ b,c ≤ n)
    and then sums, over all products a·d, the number of b·c that make
    ad – bc = n+1.  Complexity: O(n²) time, O(n²) memory (≈2 × 10⁸ bytes for n=10 000). */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Handle the offset: for n < 1 return 0 (irrelevant case). */
    if (n < 1) return 0;

    /* Size of the array holding frequency of products b * c. */
    size_t N = (size_t)n;
    size_t prod_max = N * N;                /* n²  (fits in 32‑bit for n ≤ 10 000) */

    /* Allocate zero‑initialized array of uint16_t (max count per product < 65535). */
    uint16_t *freq = (uint16_t*)calloc(prod_max + 1, sizeof(uint16_t));
    if (!freq) return 0;                    /* allocation failure → return 0 */

    /* Fill frequencies of all products b*c. */
    for (idx_t b = 1; b <= n; ++b) {
        size_t b_s = (size_t)b;
        for (idx_t c = 1; c <= n; ++c) {
            size_t p = b_s * (size_t)c;     /* product fits in size_t */
            ++freq[p];
        }
    }

    /* Sum over all a,d to count matrices with determinant n+1. */
    seq_t total = 0;
    uint64_t k = (uint64_t)n + 1;           /* det = n+1  */
    for (idx_t a_ = 1; a_ <= n; ++a_) {
        size_t a_s = (size_t)a_;
        for (idx_t d_ = 1; d_ <= n; ++d_) {
            size_t prod_ad = a_s * (size_t)d_;
            if (prod_ad <= k) continue;    /* ad – bc ≤ n => bc ≤ 0, impossible */
            size_t S = prod_ad - (size_t)k; /* S = bc  */
            if (S <= prod_max) {
                total += freq[S];
            }
        }
    }

    free(freq);
    return total;
}
