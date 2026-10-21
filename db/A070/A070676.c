/*
 * A070676  Smallest m in range 1..phi(n) such that 3^m == 1 mod n, or 0 if no
 *          such number exists.
 * https://oeis.org/A070676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   calc_phi, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal positive exponent m such that 3^m ≡ 1 (mod n); 0 if none */
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (gcd((seq_t)n, (seq_t)3) != (seq_t)1) return (seq_t)0;
    seq_t phi_val = calc_phi((seq_t)n);
    idx_t phi = (idx_t)phi_val;
    for (idx_t d = 1; d <= phi; ++d) {
        if (mod_pow((seq_t)3, d, (seq_t)n) == (seq_t)1) {
            return (seq_t)d;
        }
    }
    return (seq_t)0;
}
