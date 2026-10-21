/*
 * A211812  Number of 4-tuples (w,x,y,z) with all terms in {1,...,n} and
 *          w*x<=3*y*z.
 * https://oeis.org/A211812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4‑tuples (w,x,y,z) with all terms in {1,…,n}
 *            and w*x <= 3*y*z
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t N  = n;
    idx_t N2 = N * N;               /* n²  */

    /* cnt[p] = number of pairs (w,x) with w*x = p, 1 <= w,x <= N  */
    uint16_t *cnt = (uint16_t *)malloc((N2 + 1) * sizeof(uint16_t));
    if (!cnt) return (seq_t)0;
    memset(cnt, 0, (N2 + 1) * sizeof(uint16_t));

    for (idx_t w = 1; w <= N; ++w)
        for (idx_t x = 1; x <= N; ++x)
            ++cnt[w * x];

    seq_t total_pairs = (seq_t)N * (seq_t)N; /* n² */
    seq_t result      = 0;
    seq_t prefix_sum  = 0;                   /* sum of cnt[1 .. t],  t = floor((i-1)/3) */

    for (idx_t i = 1; i <= N2; ++i) {
        seq_t count_ge = total_pairs - prefix_sum; /* # of (y,z) with product >= ceil(i/3) */
        result += (seq_t)cnt[i] * count_ge;

        if (i % 3 == 0) {                      /* next threshold t increases */
            prefix_sum += cnt[i / 3];
        }
    }

    free(cnt);
    return result;
}
