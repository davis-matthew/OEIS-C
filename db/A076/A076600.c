/*
 * A076600  a(n) = smallest k > n such that n^2 + k^2 is a square, or 0 if no
 *          such k exists.
 * https://oeis.org/A076600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   compute smallest k>n such that n^2 + k^2 is a square. The
 *            procedure finds all divisor pairs d of n, treats n/d as a
 *            primitive leg in a primitive Pythagorean triple, then scales the
 *            corresponding other leg by d. The minimal such k>n is returned.
 *            If no such k exists, 0 is returned
 * Range:     u8 n <= 22, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, int_sqrt_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/int_sqrt_idx.h"

static inline void try_divisor(idx_t d, idx_t n, seq_t *min_k)
{
    idx_t nprime = n / d;
    /* ----- first type: nprime = p^2 - q^2 ----- */
    idx_t lim = int_sqrt_idx(nprime);
    for (idx_t y = 1; y <= lim; ++y) {
        if (nprime % y != 0) continue;
        idx_t x = nprime / y;
        if ((x & 1) != (y & 1)) continue;    /* same parity required */
        idx_t p = (x + y) >> 1;
        idx_t q = (x - y) >> 1;
        if (p <= q || q == 0) continue;
        if (gcd((seq_t)p, (seq_t)q) != 1) continue;
        if ((p & 1) && (q & 1)) continue;   /* not both odd */
        seq_t other = (seq_t)2 * (seq_t)p * (seq_t)q;
        seq_t candidate = (seq_t)d * other;
        if (candidate <= (seq_t)n) continue;
        if (*min_k == 0 || candidate < *min_k) *min_k = candidate;
    }

    /* ----- second type: nprime = 2 * p * q ----- */
    if (nprime >= 2) {
        idx_t limq = int_sqrt_idx(nprime / 2);
        for (idx_t q = 1; q <= limq; ++q) {
            if ((nprime & 1) != 0) continue; /* nprime must be even */
            if (nprime % (2 * q) != 0) continue;
            idx_t p = nprime / (2 * q);
            if (p <= q) continue;
            if (gcd((seq_t)p, (seq_t)q) != 1) continue;
            if ((p & 1) && (q & 1)) continue;   /* not both odd */
            seq_t other = (seq_t)p * (seq_t)p - (seq_t)q * (seq_t)q;
            seq_t candidate = (seq_t)d * other;
            if (candidate <= (seq_t)n) continue;
            if (*min_k == 0 || candidate < *min_k) *min_k = candidate;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;     /* 0^2 + 1^2 = 1^2 */
    seq_t min_k = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;
        try_divisor(d, n, &min_k);
        idx_t d2 = n / d;
        if (d2 != d) try_divisor(d2, n, &min_k);
    }
    return min_k;
}
