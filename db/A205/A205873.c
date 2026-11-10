/*
 * A205873  The least number j such that 9 divides s(k)-s(j), where
 *          k(n)=A205872(n) and s(k) denotes the (k+1)-st Fibonacci number.
 * https://oeis.org/A205873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j of the n-th pair (j,k), j<k, ordered by k then j, with
 *            F(k+1) == F(j+1) (mod 9); computed by direct search using the
 *            Pisano period 24 of mod 9
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A205873_res(idx_t k)
{
    static int f[24];
    static int init = 0;
    if (!init) {
        f[0] = 0; f[1] = 1;
        for (int i = 2; i < 24; i++) f[i] = (f[i-1] + f[i-2]) % 9;
        init = 1;
    }
    return f[(int)((k + 1) % 24)];
}

seq_t a(idx_t n)
{
    idx_t freq[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    idx_t rem = n;
    for (idx_t k = 1; ; ++k) {
        int r = A205873_res(k);
        idx_t c = freq[r];
        if (rem <= c) {
            idx_t seen = 0;
            for (idx_t j = 1; j < k; ++j) {
                if (A205873_res(j) == r) {
                    ++seen;
                    if (seen == rem) return (seq_t)j;
                }
            }
        }
        rem -= (rem <= c) ? rem : c;
        freq[r]++;
    }
}
