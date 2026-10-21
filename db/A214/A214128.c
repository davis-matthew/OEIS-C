/*
 * A214128  a(n) = 6^(6^6) mod n.
 * https://oeis.org/A214128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^(6^6) mod n
 * Range:     u8 n <= 263, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* n is the index (1-based).  Compute 6^46656 mod n. */
    seq_t mod = (seq_t)n;
    /* 6^6 = 46656, used as the exponent. */
    const uint64_t exp = 46656ULL;
    seq_t result = mod_pow((seq_t)6, exp, mod);
    return result;
}
