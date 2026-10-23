/*
 * A121491  An historic sequence: Lehmer's linear congruential pseudorandom
 *          numbers.
 * https://oeis.org/A121491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recursive formula:
 *  a(0) = 47594118
 *  a(n+1) = (23 * a(n)) mod 100000001
 *
 *  Equivalently: a(n) = (23^n * 47594118) mod 100000001
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* Initialize constants as seq_t */
    static const seq_t coef  = (seq_t)23;
    static const seq_t mod   = (seq_t)100000001;
    static const seq_t init  = (seq_t)47594118;

    /* Compute 23^n modulo mod */
    seq_t pow_n = mod_pow(coef, (uint64_t)n, mod);

    /* Return result */
    return (pow_n * init) % mod;
}
