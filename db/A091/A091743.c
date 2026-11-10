/*
 * A091743  Third column (m=2) of triangle A060063 divided by 9.
 * https://oeis.org/A091743
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A060063(n, 2)/9, n>=2, where row n of A060063 is the
 *            coefficient list of p(n,x) with p(0,x)=1 and p(n,x) =
 *            x*(1-x)^2*p''(n-1,x) + (1+6*(n-1)*x+(5-6*n)*x^2)*p'(n-1,x) +
 *            (3n-2)*(1+(3n-2)*x)*p(n-1,x)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 13 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *prev = mem_new(seq_t, N + 2);
    seq_t *curr = mem_new(seq_t, N + 2);
    size_t prowlen = 1;
    prev[0] = (seq_t)1;

    for (size_t next = 1; next <= N; ++next) {
        size_t next_len = next + 1;
        for (size_t k = 0; k < next_len; ++k) {
            seq_t pos = 0, neg = 0;
            seq_t m = (seq_t)(3 * next - 2);
            if (k + 1 < prowlen) {
                seq_t t = prev[k + 1];
                pos += t * (seq_t)(k + 1) * (seq_t)k;
                pos += t * (seq_t)(k + 1);
            }
            if (k < prowlen && k >= 2) {
                neg += prev[k] * (seq_t)k * (seq_t)(k - 1) * (seq_t)2;
            }
            if (k >= 3 && (k - 1) < prowlen) {
                pos += prev[k - 1] * (seq_t)(k - 1) * (seq_t)(k - 2);
            }
            if (k < prowlen) {
                pos += prev[k] * (seq_t)k * (seq_t)(6 * (next - 1));
                pos += prev[k] * m;
            }
            if (k >= 1 && (k - 1) < prowlen) {
                neg += prev[k - 1] * (seq_t)(k - 1) * (seq_t)(6 * next - 5);
                pos += prev[k - 1] * m * m;
            }
            curr[k] = pos - neg;
        }
        seq_t *tmp = prev; prev = curr; curr = tmp;
        prowlen = next_len;
    }

    seq_t res = prev[2] / 9;
    mem_free(prev);
    mem_free(curr);
    return res;
}
