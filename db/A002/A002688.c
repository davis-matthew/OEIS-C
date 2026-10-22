/*
 * A002688  Denominators of coefficients for repeated integration.
 * https://oeis.org/A002688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((1/n!) * Sum_{k=1..n}
 *            Stirling1(n,k)/((k+1)*(k+2))). Since 1/((k+1)(k+2)) = Int_0^1
 *            t^k (1-t) dt, the g.f. is x/log(1+x)^2 - 1/log(1+x), which gives
 *            a(n) = denominator(-(n+1)*G(n+1) - (n-1)*G(n)), where G(m) =
 *            (1/m!) * Sum_{k=1..m} Stirling1(m,k)/(k+1) are the Gregory
 *            coefficie...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   a2688_big, a2688_fixed, a2688_fixed_build, a2688_igcd,
 *            a2688_inv, a2688_mulmod, a2688_sgcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/a2688_big.h"
#include "../utils/a2688_fixed.h"
#include "../utils/a2688_fixed_build.h"
#include "../utils/a2688_igcd.h"
#include "../utils/a2688_inv.h"
#include "../utils/a2688_mulmod.h"
#include "../utils/a2688_sgcd.h"

seq_t a(idx_t n) {
    if (a2688_fixed_build()) return a2688_fixed(n);
    return a2688_big(n);
}
