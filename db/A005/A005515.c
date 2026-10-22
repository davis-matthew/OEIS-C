/*
 * A005515  Number of n-bead bracelets (turnover necklaces) of two colors with
 *          10 red beads and n-10 black beads.
 * https://oeis.org/A005515
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (C(n-1,9) + [2|n]*C(n/2-1,4) + 4*[5|n]*(n/5-1) + 4*[10|n]
 *            + 10*C(floor((n-10)/2)+5,5)) / 20 (Burnside: necklaces
 *            (1/10)*Sum_{d|10} phi(d)*C(n/d-1,10/d-1), averaged with the
 *            reflection count from the term (1+x)/(1-x^2)^6 in the OEIS g.f.)
 * Range:     u8 n <= 15, u16 n <= 24, u32 n <= 73, u64 n <= 804,
 *            u128 n <= 1000, big any n
 * Helpers:   binom_u128, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/binom_u128.h"

static seq_t binom_seq(idx_t m, idx_t k) {
    seq_t c = 1;
    idx_t i;
    if (k > m) return 0;
    for (i = 1; i <= k; i++)
        c = c * (seq_t)(m - k + i) / (seq_t)i;
    return c;
}

seq_t a(idx_t n) {
    if (n < 10) return 0;
    if (n <= 20000) {
        unsigned __int128 s = binom_u128(n - 1, 9);
        if (n % 2 == 0) s += binom_u128(n / 2 - 1, 4);
        if (n % 5 == 0) s += 4 * (unsigned __int128)(n / 5 - 1);
        if (n % 10 == 0) s += 4;
        s += 10 * binom_u128((n - 10) / 2 + 5, 5);
        return u128_to_seq(s / 20);
    } else {
        seq_t s = binom_seq(n - 1, 9);
        if (n % 2 == 0) s += binom_seq(n / 2 - 1, 4);
        if (n % 5 == 0) s += 4 * ((seq_t)(n / 5) - 1);
        if (n % 10 == 0) s += 4;
        s += 10 * binom_seq((n - 10) / 2 + 5, 5);
        return s / 20;
    }
}
