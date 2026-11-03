/*
 * A006079  Number of asymmetric planted projective plane trees with n+1
 *          nodes; bracelets (reversible necklaces) with n black beads and n-1
 *          white beads.
 * https://oeis.org/A006079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(1) = a(2) = 1; a(n+1) = (CatalanNumber(n) -
 *            binomial(n,floor(n/2)))/2 for n >= 2, where Catalan(k+1) =
 *            Catalan(k)*(4k+2)/(k+2), Catalan(0) = 1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 189 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static seq_t a006079_big(uint64_t m) {
    seq_t c = 1, b = 1;
    uint64_t k, j, h = m / 2;
    for (k = 0; k < m; k++) {
        uint64_t num = 4 * k + 2, den = k + 2;
        uint64_t g = gcd_u64(num, den);
        num /= g; den /= g;
        c = c / (seq_t)den * (seq_t)num;
    }
    for (j = 0; j < h; j++) {
        uint64_t num = m - j, den = j + 1;
        uint64_t g = gcd_u64(num, den);
        num /= g; den /= g;
        b = b / (seq_t)den * (seq_t)num;
    }
    return (c - b) / 2;
}

/* Fixed-width builds: work in 128 bits so Catalan(m) may exceed seq_t
   while the halved difference still fits. */
static unsigned __int128 a006079_u128(uint64_t m) {
    unsigned __int128 c = 1, b = 1;
    uint64_t k, j, h = m / 2;
    for (k = 0; k < m; k++) {
        uint64_t num = 4 * k + 2, den = k + 2;
        uint64_t g = gcd_u64(num, den);
        num /= g; den /= g;
        c = c / den * num;
    }
    for (j = 0; j < h; j++) {
        uint64_t num = m - j, den = j + 1;
        uint64_t g = gcd_u64(num, den);
        num /= g; den /= g;
        b = b / den * num;
    }
    return (c - b) / 2;
}

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    if (seq_is_fixed_width()) return u128_to_seq(a006079_u128((uint64_t)(n - 1)));
    return a006079_big((uint64_t)(n - 1));
}
