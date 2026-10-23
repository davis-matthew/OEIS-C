/*
 * A159783  Hankel transform of ordered Bell numbers A000670.
 *          a(n)=det(A000670(i+j-1)), i,j=1,2..n.
 * https://oeis.org/A159783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((n-1)*n/2) * A000178(n-1) * A000178(n)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where A000178(k) = Π_{i=1}^k i!  (superfactorial) */

#include <stddef.h>

/* Compute 2^exp using exponentiation by squaring, all in seq_t arithmetic. */
static seq_t pow2_idx(idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp > 0) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* safety for index below offset */

    /* compute superfactorial(n-1) and superfactorial(n) */
    seq_t fact      = (seq_t)1;    /* current factorial */
    seq_t super     = (seq_t)1;    /* superfac(current i) */
    seq_t super_prev = (seq_t)1;   /* superfac(i-1) */

    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;          /* factorial = i! */
        super_prev = super;        /* store previous superfac */
        super *= fact;             /* superfac(i) = superfac(i-1) * i! */
    }

    /* exponent for the power of two */
    idx_t e = n * (n - 1) / 2;

    seq_t twopow = pow2_idx(e);
    seq_t result = twopow;
    result *= super_prev;
    result *= super;
    return result;
}
