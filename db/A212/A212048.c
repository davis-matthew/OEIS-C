/*
 * A212048  Number of (w,x,y,z) with all terms in {1,...,n} and 3w*x=4*y*z.
 * https://oeis.org/A212048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(w,x,y,z) : 1≤w,x,y,z≤n and 3 w x = 4 y z}
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint16_t *B3 = NULL;          /* B3[k] = #pairs (y,z) with y*z = 3*k (≤n²) */
static uint16_t *A4 = NULL;          /* A4[k] = #pairs (w,x) with w*x = 4*k (≤n²) */
static size_t B3_size = 0;
static size_t A4_size = 0;

seq_t a(idx_t n)
{
    /* Largest possible k so that 4*k ≤ n² (and also 3*k ≤ n²) */
    size_t kmax = ((size_t)n * (size_t)n) / 4;

    /* Re‑allocate if the required range has grown */
    if (kmax + 1 > B3_size) {
        size_t new_size = kmax + 1;
        free(B3);
        B3 = (uint16_t *)malloc(new_size * sizeof(uint16_t));
        if (!B3) return 0;                 /* allocation failure */
        B3_size = new_size;
        free(A4);
        A4 = (uint16_t *)malloc(new_size * sizeof(uint16_t));
        if (!A4) return 0;
        A4_size = new_size;
    }

    /* zero the counting arrays */
    memset(B3, 0, B3_size * sizeof(uint16_t));
    memset(A4, 0, A4_size * sizeof(uint16_t));

    /* Count (y,z) pairs: y * z must be a multiple of 3 */
    for (idx_t y = 1; y <= n; ++y) {
        size_t yv = (size_t)y;
        for (idx_t z = 1; z <= n; ++z) {
            size_t prod = yv * (size_t)z;          /* product ≤ n² ≤ 1e8 */
            if (prod % 3 == 0) {
                size_t k = prod / 3;
                if (k <= kmax)
                    B3[k] += 1;
            }
        }
    }

    /* Count (w,x) pairs: w * x must be a multiple of 4 */
    for (idx_t w = 1; w <= n; ++w) {
        size_t wv = (size_t)w;
        for (idx_t x = 1; x <= n; ++x) {
            size_t prod = wv * (size_t)x;
            if (prod % 4 == 0) {
                size_t k = prod / 4;
                if (k <= kmax)
                    A4[k] += 1;
            }
        }
    }

    /* Sum over all k to get the final count */
    seq_t ans = 0;
    for (size_t k = 1; k <= kmax; ++k) {
        if (B3[k] && A4[k])
            ans += (seq_t)B3[k] * (seq_t)A4[k];
    }
    return ans;
}
