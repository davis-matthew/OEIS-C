/*
 * A001371  Number of n-bead necklaces with beads of 2 colors and primitive
 *          period n, when turning over is allowed.
 * https://oeis.org/A001371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{d|n} mu(d)*A000029(n/d), a(0) = 1, where A000029(m)
 *            = (1/(2m)) * Sum_{d|m} phi(d)*2^(m/d) + (2^((m-1)/2) if m odd,
 *            3*2^(m/2-2) if m even)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 71, u128 n <= 136,
 *            big any n
 * Helpers:   l_bracelets, l_mu, l_phi, l_pow2, l_pow2_divmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/l_bracelets.h"
#include "../utils/l_mu.h"
#include "../utils/l_phi.h"
#include "../utils/l_pow2.h"
#include "../utils/l_pow2_divmod.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t pos = 0, neg = 0;
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d != 0) continue;
        idx_t e = n / d;
        int mu = l_mu(d);
        if (mu > 0) pos += l_bracelets(e);
        else if (mu < 0) neg += l_bracelets(e);
        if (e != d) {
            mu = l_mu(e);
            if (mu > 0) pos += l_bracelets(d);
            else if (mu < 0) neg += l_bracelets(d);
        }
    }
    return pos - neg;
}
