/*
 * A004733  Denominator of n!!/(n+3)!!.
 * https://oeis.org/A004733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(2m) = (2m+1)*(2m+3)*binomial(2m,m)/2^A000120(m); a(2m+1) =
 *            2^(2m+4-A000120(m+2))
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   a004733_gcd_u64, a004733_popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004733_gcd_u64.h"
#include "../utils/a004733_popcount.h"

seq_t a(idx_t n) {
    if (n % 2 == 1) {
        idx_t m = (n - 1) / 2;
        idx_t e = 2 * m + 4 - (idx_t)a004733_popcount(m + 2);
        seq_t r = 1;
        for (idx_t i = 0; i < e; i++) r *= 2;
        return r;
    }
    idx_t m = n / 2;
    /* O = odd part of binomial(2k,k), updated by O_k = O_{k-1}*(2k-1)/oddpart(k) */
    seq_t o = 1;
    for (idx_t k = 1; k <= m; k++) {
        uint64_t kp = k;
        while (kp % 2 == 0) kp /= 2;
        uint64_t rem = (uint64_t)(o % (seq_t)kp);
        uint64_t g = a004733_gcd_u64(rem, kp);
        o /= (seq_t)g;
        uint64_t q = (2 * k - 1) / (kp / g);
        o *= (seq_t)q;
    }
    o *= (seq_t)(2 * m + 1);
    o *= (seq_t)(2 * m + 3);
    return o;
}
