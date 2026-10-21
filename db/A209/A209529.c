/*
 * A209529  Half the number of (n+1)X(n+1) 0..2 arrays with every 2X2 subblock
 *          having exactly two distinct clockwise edge differences.
 * https://oeis.org/A209529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*m^2) + 1 if n = 2*m - 1 (odd n) * a(n) = 3 *
 *            2^(2*m*(m+1) - 1) + 1 if n = 2*m (even n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   _even_exponent, _odd_exponent, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/_even_exponent.h"
#include "../utils/_odd_exponent.h"

seq_t a(idx_t n) {
    /* For n >= 1, return the sequence value. */
    if (n == 0) return 0; /* optional guard for non‑positive index */

    if ((n & 1ULL) == 0) { /* even n: n = 2*m */
        idx_t exp = _even_exponent(n);
        seq_t val = big_pow(2, exp);          /* 2^exp */
        val = val * 3 + 1;                    /* 3*2^exp + 1 */
        return val;
    } else { /* odd n: n = 2*m - 1 */
        idx_t exp = _odd_exponent(n);
        seq_t val = big_pow(2, exp);          /* 2^exp */
        val = val + 1;                        /* 2^exp + 1 */
        return val;
    }
}
