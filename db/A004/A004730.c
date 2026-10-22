/*
 * A004730  Numerator of n!!/(n+1)!! (cf. A006882).
 * https://oeis.org/A004730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(2m) = 2^(2m - A000120(m)), a(2m+1) = A000984(m+1) /
 *            2^A000120(m+1), i.e. the reduced numerator of n!!/(n+1)!!
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   a004730_gcd, a004730_popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004730_gcd.h"
#include "../utils/a004730_popcount.h"

seq_t a(idx_t n) {
    if (n % 2 == 0) {
        idx_t m = n / 2;
        idx_t e = 2 * m - a004730_popcount(m);
        seq_t r = 1;
        for (idx_t i = 0; i < e; i++) r *= 2;
        return r;
    } else {
        /* odd part of C(2k,k), k = (n+1)/2, built from
           C(2i,i) = C(2i-2,i-1) * 2(2i-1)/i */
        idx_t k = (n + 1) / 2;
        seq_t r = 1;
        for (idx_t i = 1; i <= k; i++) {
            idx_t oi = i;
            while (oi % 2 == 0) oi /= 2;
            seq_t d = (seq_t)oi;
            seq_t g = a004730_gcd(r, d);
            r /= g;
            d /= g;
            r *= ((seq_t)(2 * i - 1)) / d;
        }
        return r;
    }
}
