/*
 * A032843  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^i has d(m)
 *          < d(m-1) > d(m-2) < ...
 * https://oeis.org/A032843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   OEIS A032843 – numbers whose base‑5 digits alternate up and
 *            down: d₀ < d₁ > d₂ < d₃ > … (d₀ is the most significant digit)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   compute_dp_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_dp_len.h"

static unsigned long long count_len(size_t len, unsigned long long *dp)
{
    compute_dp_len(len, dp);
    unsigned long long total = 0ULL;
    for (int d = 1; d < 5; ++d)
        total += dp[d];                      /* dp[0*5 + d] */
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of range (offset 1) */

    idx_t          target = n;
    size_t         len   = 1;
    idx_t          cumulative = 0;
    unsigned long long *dp = NULL;

    /* Find length of the target number */
    while (1) {
        dp = (unsigned long long *)malloc(len * 5 * sizeof(unsigned long long));
        if (!dp) return (seq_t)0;           /* allocation failure */
        unsigned long long total = count_len(len, dp);
        if (cumulative + total >= target)
            break;
        cumulative += total;
        free(dp);
        ++len;
    }

    /* Rank within numbers of this length (0‑based) */
    idx_t rank = target - cumulative - 1;

    /* Prepare digits array */
    uint8_t *digits = (uint8_t *)malloc(len * sizeof(uint8_t));
    if (!digits) { free(dp); return (seq_t)0; }

    uint8_t prev = 0;

    /* Generate digits from most significant to least significant */
    for (size_t pos = 0; pos < len; ++pos) {
        for (int d = (pos == 0 ? 1 : 0); d < 5; ++d) {

            if (pos > 0) {
                if ((pos - 1) % 2 == 0) {          /* prev < d */
                    if (!(prev < d)) continue;
                } else {                          /* prev > d */
                    if (!(prev > d)) continue;
                }
            }

            unsigned long long cnt = dp[pos * 5 + d];
            if (rank < cnt) {
                digits[pos] = (uint8_t)d;
                prev = (uint8_t)d;
                break;
            } else {
                rank -= cnt;
            }
        }
    }

    free(dp);

    /* Convert digits to a seq_t value */
    seq_t val = 0;
    for (size_t i = 0; i < len; ++i)
        val = val * (seq_t)5 + (seq_t)digits[i];

    free(digits);
    return val;
}
