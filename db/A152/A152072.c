/*
 * A152072  Triangle read by rows: T(n, k) is the largest product of a
 *          partition of n into k positive integers.
 * https://oeis.org/A152072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k), where (r,k) are the row and column of n in the
 *            triangle read by rows. T(r,k) = \prod_{i=0}^{k-1} \lfloor
 *            (r+i)/k \rfloor = (q+1)^rem * q^(k-rem) with q = r/k, rem = r%k.
 *            Values are computed exactly using integer arithmetic; auxiliary
 *            functions seq_mul and seq_pow are used for multi...
 * Range:     u8 n <= 123, u16 n <= 474, u32 n <= 1849, u64 n <= 7420,
 *            u128 n <= 9749, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* One line comment explaining method, since this is not a recurrence */
#include <stdbool.h>
#include "../utils/seq_mul.h"

/* Compute base^exp using only seq_mul, safe for small exponents. */
static inline seq_t power(seq_t base, idx_t exp) {
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1u)
            res = seq_mul(res, base);
        base = seq_mul(base, base);
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* offset is 1, return 0 for safety */

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    idx_t tri = 0;
    while (tri < n) {
        ++r;
        tri += r;      /* now tri = r*(r+1)/2 */
    }

    /* Column within row */
    idx_t k = n - (tri - r);   /* k in [1..r] */

    /* Compute optimal partition product */
    seq_t q = r / k;
    idx_t rem = r % k;

    seq_t base_big = q + (seq_t)1;  /* ceil part */
    seq_t base_small = q;           /* floor part */

    seq_t part1 = power(base_big, rem);
    seq_t part2 = power(base_small, k - rem);

    return seq_mul(part1, part2);
}
