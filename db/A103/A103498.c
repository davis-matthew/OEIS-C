/*
 * A103498  Multiplicative suborder of 12 (mod 2n+1) = sord(12, 2n+1).
 * https://oeis.org/A103498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = minimum e for which 12^e ≡ ±1 (mod 2n+1), or 0 if no such
 *            e exists
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   compute_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_phi.h"

seq_t a(idx_t n) {
    uint64_t m = 2 * n + 1;
    if (m == 1) return (seq_t)0;
    if (m % 3 == 0) return (seq_t)0;   /* 12 shares factor 3 → cannot be ±1 mod m */

    uint64_t phi_m = compute_phi(m);
    uint64_t val = 12 % m;
    uint64_t e = 1;
    if (val == 1 || val == m - 1) return (seq_t)e;

    while (e < phi_m) {
        val = (val * 12) % m;
        ++e;
        if (val == 1 || val == m - 1) return (seq_t)e;
    }
    return (seq_t)0;   /* no exponent found within the group order */
}
