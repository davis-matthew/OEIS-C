/*
 * A006175  Witt vector *4!.
 * https://oeis.org/A006175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((4n)!/(n!)^4 - Sum_{d|n, d<n} d*a(d)^(n/d)) / n, i.e.
 *            Sum_{d|n} d*a(d)^(n/d) = (4n)!/(n!)^4 (Witt ghost components),
 *            equivalent to Witt transform of A074652 via b(n) = Sum_{d|n}
 *            A074650(n/d, a(d))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   a6175_binom, a6175_ghost, a6175_ipw, seq_is_fixed_width,
 *            u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/a6175_binom.h"
#include "../utils/a6175_ghost.h"
#include "../utils/a6175_ipw.h"

#define A6175_MAXD 8192

seq_t a(idx_t n)
{
    static idx_t D[A6175_MAXD];
    seq_t v[A6175_MAXD];
    idx_t lo[A6175_MAXD / 2 + 1], hi[A6175_MAXD / 2 + 1];
    int nlo = 0, nhi = 0, nd = 0, i, j;
    idx_t q;
    bool fixed = seq_is_fixed_width();

    for (q = 1; q * q <= n; q++) {
        if (n % q == 0) {
            lo[nlo++] = q;
            if (q != n / q) hi[nhi++] = n / q;
        }
    }
    for (i = 0; i < nlo; i++) D[nd++] = lo[i];
    for (i = nhi - 1; i >= 0; i--) D[nd++] = hi[i];

    for (i = 0; i < nd; i++) {
        idx_t d = D[i];
        seq_t s = a6175_ghost(d);
        for (j = 0; j < i; j++) {
            if (d % D[j] == 0)
                s -= (seq_t)D[j] * a6175_ipw(v[j], d / D[j]);
        }
        if (fixed) {
            idx_t m = d;
            int k = 0;
            while ((m & 1) == 0) { m >>= 1; k++; }
            if (k) s >>= k;
            if (m > 1) {
                unsigned __int128 mm = (unsigned __int128)m, x = mm;
                int t;
                for (t = 0; t < 7; t++) x *= (unsigned __int128)2 - mm * x;
                s *= u128_to_seq(x);
            }
        } else {
            s /= (seq_t)d;
        }
        v[i] = s;
    }
    return v[nd - 1];
}
