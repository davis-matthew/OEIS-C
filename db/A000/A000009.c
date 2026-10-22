/*
 * A000009  Expansion of Product_{m >= 1} (1 + x^m); number of partitions of n
 *          into distinct parts; number of partitions of n into odd parts.
 * https://oeis.org/A000009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   Product_{m>=1}(1+x^m) * Product_{m>=1}(1-x^m) =
 *            Product_{m>=1}(1-x^(2m)), so with Euler's pentagonal theorem:
 *            a(n) = e(n) + Sum_{k>=1} (-1)^(k+1) * (a(n-k(3k-1)/2) +
 *            a(n-k(3k+1)/2)), a(0) = 1, where e(n) = (-1)^j if n = j(3j-1) or
 *            n = j(3j+1) for some j >= 0, else 0
 * Range:     u8 n <= 28, u16 n <= 78, u32 n <= 237, u64 n <= 791,
 *            u128 n <= 2821, big any n
 * Helpers:   a9_e2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a9_e2.h"

#define A9_CAP 65536

static seq_t a9_cache[A9_CAP];
static idx_t a9_filled = 0;

seq_t a(idx_t n)
{
    while (a9_filled <= n && a9_filled < A9_CAP) {
        idx_t i = a9_filled;
        seq_t pos = 0, neg = 0;
        idx_t k;
        int e = a9_e2(i);
        if (e > 0) pos += 1;
        else if (e < 0) neg += 1;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > i) break;
            if (k & 1) {
                pos += a9_cache[i - g1];
                if (g2 <= i) pos += a9_cache[i - g2];
            } else {
                neg += a9_cache[i - g1];
                if (g2 <= i) neg += a9_cache[i - g2];
            }
        }
        a9_cache[i] = pos - neg;
        a9_filled++;
    }
    return a9_cache[n];
}
