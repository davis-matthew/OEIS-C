/*
 * A129799  a(n) = floor(9^n/n).
 * https://oeis.org/A129799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(9^n / n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Avoid overflow for 64‑bit seq_t by using a 128‑bit intermediate.  */
    if (sizeof(seq_t) > 8) {
        /* For larger seq_t types (128‑bit or arbitrary precision) use the
           provided big_pow helper, which returns the full power. */
        seq_t p = big_pow((seq_t)9, (seq_t)n);
        return p / (seq_t)n;
    } else {
        unsigned __int128 v = 1;
        for (idx_t i = 0; i < n; ++i) {
            v *= 9;
        }
        v /= n;                 /* truncating division = floor for positive */
        return (seq_t)v;
    }
}
