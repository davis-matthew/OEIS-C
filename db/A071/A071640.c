/*
 * A071640  a(n) = Sum_{i=1..n} A040051(i).
 * https://oeis.org/A071640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} A040051(i), where A040051(i) = parity of
 *            p(i), computed by the pentagonal recurrence mod 2: A040051(0) =
 *            1, A040051(m) = XOR over k>=1 of A040051(m-k(3k-1)/2) and
 *            A040051(m-k(3k+1)/2)
 * Range:     u8 n <= 465, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 90 terms through n = 902 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *par = NULL;
    static seq_t *sum = NULL;
    static idx_t size = 0;

    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *p2 = mem_resize(seq_t, par, new_size);
        if (!p2) return 0;
        par = p2;
        seq_t *s2 = mem_resize(seq_t, sum, new_size);
        if (!s2) return 0;
        sum = s2;
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0) {
                par[0] = 1;
                sum[0] = 0;
                continue;
            }
            unsigned v = 0;
            for (idx_t k = 1; ; ++k) {
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > i) break;
                v ^= (unsigned)(par[i - g1] & 1);
                idx_t g2 = k * (3 * k + 1) / 2;
                if (g2 <= i) v ^= (unsigned)(par[i - g2] & 1);
            }
            par[i] = (seq_t)(v & 1);
            sum[i] = sum[i - 1] + par[i];
        }
        size = new_size;
    }
    return sum[n];
}
