/*
 * A050624  Let b(n) = A050623(n) = smallest n-digit number divisible by 3^n;
 *          sequence gives b(n)/3^n.
 * https://oeis.org/A050624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil(10^(n-1) / 3^n)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint128.h"

/* Main sequence function. Works for any seq_t:
   - If seq_t is 64‑bit or smaller, uses 128‑bit arithmetic to avoid
     intermediate overflow.
   - If seq_t is larger (e.g. arbitrary‑precision), falls back to
     direct seq_t operations, which never overflow. */
seq_t a (idx_t n)
{
    /* n >= 1 by definition of the sequence */
    if (n == 0) return 0;  /* not needed for official offset, safe guard */

    /* Narrowed path for small seq_t (<=64 bits) */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        unsigned __int128 pow10 = pow_uint128(10, n-1);   /* 10^(n-1)   */
        unsigned __int128 pow3  = pow_uint128( 3, n);     /* 3^n         */
        unsigned __int128 numerator = pow10 + pow3 - 1;   /* (10^(n-1)+3^n-1) */
        unsigned __int128 res128   = numerator / pow3;
        return (seq_t)res128;
    }

    /* Generic path for larger seq_t (arbitrary‑precision types). */
    seq_t pow10 = 1;
    seq_t base = 10;
    idx_t exp = n-1;
    while (exp) {
        if (exp & 1) pow10 *= base;
        base *= base;
        exp >>= 1;
    }

    seq_t pow3 = 1;
    base = 3;
    exp = n;
    while (exp) {
        if (exp & 1) pow3 *= base;
        base *= base;
        exp >>= 1;
    }

    seq_t num = pow10 + pow3 - 1;
    seq_t res = num / pow3;
    return res;
}
