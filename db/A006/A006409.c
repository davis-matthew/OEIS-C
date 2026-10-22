/*
 * A006409  Number of nonseparable rooted toroidal maps with n + 4 edges and n
 *          + 1 vertices.
 * https://oeis.org/A006409
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10 * binomial(n + 4, 6) + 120 * binomial(n + 4, 7) + 328
 *            * binomial(n + 4, 8) + 232 * binomial(n + 4, 9)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 25, u64 n <= 311, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    idx_t m = n + 4;
    seq_t result = 0;
    
    result += (seq_t)10 * binom(m, 6);
    result += (seq_t)120 * binom(m, 7);
    result += (seq_t)328 * binom(m, 8);
    result += (seq_t)232 * binom(m, 9);
    
    return result;
}
