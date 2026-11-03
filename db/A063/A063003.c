/*
 * A063003  Difference between 3^n and the next larger or equal power of 2.
 * https://oeis.org/A063003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 178 (2026-11-01)
 * Helpers:   bit_length, is_pow2, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2^(ceil(log2(3^n))) - 3^n
 * Computes the difference between 3^n and the next larger or equal power of 2.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/is_pow2.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t p = pow3(n);                     /* 3^n */
    unsigned bl = bit_length(p);         /* bit length of 3^n */
    idx_t exp = bl - 1;                    /* floor(log2(3^n)) */

    seq_t pw = pow2(exp);                  /* 2^exp */
    if (!is_pow2(p)) {             /* if 3^n is not a power of two */
        ++exp;                             /* move to the next exponent */
        pw = pow2(exp);                    /* 2^(exp+1) */
    }

    seq_t res = pw;                        /* start with 2^ceil(log2(3^n)) */
    res -= p;                              /* subtract 3^n */

    return res;
}
