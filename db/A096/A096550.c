/*
 * A096550  Consecutive internal states of the IMSL pseudo-random number
 *          generator RNUN when started with ISEED=1.
 * https://oeis.org/A096550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modmult
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A096550
 *
 *  Formula: a(1) = 1
 *  Recurrence: a(n) = 16807 * a(n-1) mod 2147483647    (n ≥ 2)
 *
 *  The LCG multiplier 16807 is 7^5.  The modulus 2147483647 is 2^31‑1.
 *  The function computes a(n) by fast exponentiation:
 *          a(n) = 16807^(n-1) mod 2147483647
 *  This works for any n ≥ 1 and is independent of call order.
 */
#include <stdint.h>
#include "../utils/modmult.h"

seq_t a(idx_t n)
{
    /* The modulus and multiplier fit in 32 bits; all intermediate
     * values are cast to 64 bits for safe multiplication. */
    const seq_t MOD = (seq_t)2147483647U;   /* 2^31‑1, a Mersenne prime */
    const seq_t FACTOR = (seq_t)16807U;     /* 7^5 */

    if (n == (idx_t)0) return (seq_t)0;    /* undefined, but guard against 0 */

    seq_t result = (seq_t)1;               /* FACTOR^(n‑1) */
    seq_t base   = FACTOR;
    idx_t exp = n - (idx_t)1;

    while (exp > 0) {
        if (exp & 1U) result = modmult(result, base, MOD);
        base = modmult(base, base, MOD);
        exp >>= 1U;
    }

    return result;
}
