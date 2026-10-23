/*
 * A100693  Number of self-avoiding paths with n steps on a hexagonal lattice
 *          in the strip Z x {0,1,2}.
 * https://oeis.org/A100693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n<8 the values are fixed: 1,2,3,5,6,7,9,14. For n>=8, let m
 *            = n / 4 (integer division) and r = n % 4. If r==0 or r==1: a(n)
 *            = 7 * 2^(m-1) If r==2: a(n) = 11 * 2^(m-1) If r==3: a(n) = 15 *
 *            2^(m-1)
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* small fixed prefix */
    static const seq_t prefix[8] = {
        (seq_t)1,  /* a(0) */
        (seq_t)2,  /* a(1) */
        (seq_t)3,  /* a(2) */
        (seq_t)5,  /* a(3) */
        (seq_t)6,  /* a(4) */
        (seq_t)7,  /* a(5) */
        (seq_t)9,  /* a(6) */
        (seq_t)14  /* a(7) */
    };

    if (n < 8)
        return prefix[n];

    idx_t m = n / 4;          /* floor(n/4), m >= 2 for n >= 8 */
    idx_t r = n % 4;
    seq_t base = pow2(m - 1); /* 2^(m-1) */

    if (r == 0 || r == 1)
        return (seq_t)7 * base;
    else if (r == 2)
        return (seq_t)11 * base;
    else /* r == 3 */
        return (seq_t)15 * base;
}
