/*
 * A130119  a(n) = gcd(n^2 - 19, 45).
 * https://oeis.org/A130119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n^2 - 19, 45)
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Work modulo 45 to keep values tiny */
    seq_t n_mod = (seq_t)(n % (idx_t)45);          /* n mod 45      */
    seq_t n2_mod = (n_mod * n_mod) % (seq_t)45;    /* n^2 mod 45    */
    seq_t val_mod = (n2_mod + (seq_t)45 - (seq_t)19) % (seq_t)45;
    return gcd(val_mod, (seq_t)45);
}
