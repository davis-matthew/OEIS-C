/*
 * A209994  Number of 2 X 2 matrices with all elements in {1,2,...,n} and
 *          determinant in {-1,0,1}.
 * https://oeis.org/A209994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_p freq(p)^2 + 2 Σ_p freq(p)·freq(p+1) where freq(p) =
 *            #{(a,d) : 1 ≤ a,d ≤ n , a·d = p}. This counts all 2×2 matrices
 *            with entries in {1,…,n} whose determinant lies in {–1,0,1}. The
 *            algorithm builds the frequency table for all products 1…n² and
 *            then evaluates the two sums
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Size of frequency array = n*n + 1 (index 0 unused). */
    size_t len = (size_t)n * (size_t)n + 1;

    /* Allocate zero‑initialised array of uint16_t to hold frequencies. */
    uint16_t *freq = (uint16_t *)calloc(len, sizeof(uint16_t));
    if (!freq)
        return 0;   /* Out of memory – return 0 for safety. */

    /* Build the frequency table. */
    for (idx_t a = 1; a <= n; ++a) {
        size_t base = (size_t)a;
        for (idx_t d = 1; d <= n; ++d) {
            size_t idx = base * (size_t)d;
            /* The number of pairs (a,d) with product idx is small
               (≤ n), so a uint16_t is sufficient. */
            freq[idx] += 1;
        }
    }

    /* Compute the two sums. */
    seq_t sum_sq  = 0;
    seq_t cross   = 0;
    for (size_t p = 1; p < len; ++p) {
        uint16_t fp = freq[p];
        sum_sq += (seq_t)fp * (seq_t)fp;
        if (p + 1 < len) {
            uint16_t fp1 = freq[p + 1];
            cross += (seq_t)fp * (seq_t)fp1;
        }
    }

    free(freq);
    return sum_sq + (seq_t)2 * cross;
}
