/*
 * A032911  Numbers whose set of base-4 digits is a subset of {1,3}.
 * https://oeis.org/A032911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the unique integer whose base‑4 digits are a subset of
 *            {1,3}; for each n≥1, let L = ⌊log₂(n+1)⌋ - 1 (number of base‑4
 *            digits). Set offset = n – (2^L – 1). Interpreting offset as an
 *            L‑bit binary string, replace each 0 with digit 1 and each 1 with
 *            digit 3, then interpret this string as a ba...
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bits_of_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bits_of_idx.h"

seq_t a(idx_t n) {
    /* n ≥ 1 by contract */
    idx_t n_plus1 = n + 1;
    unsigned bits = bits_of_idx(n_plus1);
    unsigned L = bits - 1;                /* number of base‑4 digits */

    idx_t start = (idx_t)1 << L;          /* 2^L  */
    idx_t offset = n - (start - 1);       /* 0 … 2^L-1 */

    seq_t result = 0;
    for (int i = (int)L - 1; i >= 0; --i) {
        seq_t bit = (offset >> i) & 1;      /* 0 or 1 */
        seq_t digit = 1 + (bit << 1);       /* 1 if bit=0, 3 if bit=1 */
        result = result * 4 + digit;
    }
    return result;
}
