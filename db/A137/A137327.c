/*
 * A137327  Fermat(n) modulo n.
 * https://oeis.org/A137327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(2^n) + 1) mod n
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul64.h"

seq_t a(idx_t n) {
    uint64_t mod = (uint64_t)n;       /* n ≥ 1 */
    uint64_t r   = 2 % mod;           /* 2^(2^0) mod n */
    for (idx_t i = 0; i < n; ++i) {
        r = mul64(r, r, mod);         /* r = r^2 mod n → 2^(2^(i+1)) mod n */
    }
    uint64_t res = (r + 1) % mod;     /* (2^(2^n) + 1) mod n */
    return (seq_t)res;
}
