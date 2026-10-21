/*
 * A063003  Difference between 3^n and the next larger or equal power of 2.
 * https://oeis.org/A063003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   bitlen_seq_t, is_power_of_two, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2^(ceil(log2(3^n))) - 3^n
 * Computes the difference between 3^n and the next larger or equal power of 2.
 */

#include "../utils/pow3.h"            /* pow3(idx_t) */
#include "../utils/pow2.h"            /* pow2(idx_t) */
#include "../utils/bitlen_seq_t.h"    /* bitlen_seq_t(seq_t) */
#include "../utils/is_power_of_two.h" /* is_power_of_two(seq_t) */

seq_t a(idx_t n)
{
    seq_t p = pow3(n);                     /* 3^n */
    unsigned bl = bitlen_seq_t(p);         /* bit length of 3^n */
    idx_t exp = bl - 1;                    /* floor(log2(3^n)) */

    seq_t pw = pow2(exp);                  /* 2^exp */
    if (!is_power_of_two(p)) {             /* if 3^n is not a power of two */
        ++exp;                             /* move to the next exponent */
        pw = pow2(exp);                    /* 2^(exp+1) */
    }

    seq_t res = pw;                        /* start with 2^ceil(log2(3^n)) */
    res -= p;                              /* subtract 3^n */

    return res;
}
