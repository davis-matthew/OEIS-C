/*
 * A224654  Number of (n+2)X3 0..2 matrices with each 3X3 subblock having the
 *          same population.
 * https://oeis.org/A224654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (OEIS A224654):
    Let N = n + 2.  Write N = 3*m + r with 0 ≤ r < 3.
    For t ≥ 0 define
        A(t) = 3 + 6·3^t + 6^t             (counts of 3‑length rows)
    Then
        a(n) =
            A(m)^3                     if r == 0
            A(m+1) · A(m)^2            if r == 1
            A(m+1)^2 · A(m)            if r == 2
    The sequences of row sum‑patterns are 3‑periodic and the counts for a
    single row with frequencies (c0,c1,c2) are 
        r(c0,c1,c2) = 3!/(c0! c1! c2!)  ∈ {1,3,6}.
*/

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t power = base;
    while (exp > 0) {
        if (exp & 1) result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* N = n + 2 rows.  Divide by 3 to obtain m and remainder r. */
    idx_t N = n + 2;
    idx_t m = N / 3;
    idx_t r = N % 3;

    /* Compute 3^m, 6^m, 3^{m+1}, 6^{m+1}. */
    seq_t pow3_m  = pow_seq((seq_t)3, m);
    seq_t pow6_m  = pow_seq((seq_t)6, m);
    seq_t pow3_m1 = pow3_m * (seq_t)3;
    seq_t pow6_m1 = pow6_m * (seq_t)6;

    /* A(m) and A(m+1). */
    seq_t A_m   = (seq_t)3 + (seq_t)6 * pow3_m + pow6_m;
    seq_t A_m1  = (seq_t)3 + (seq_t)6 * pow3_m1 + pow6_m1;

    /* Compute a(n) according to the remainder. */
    seq_t result;
    if (r == 0) {
        result = A_m * A_m * A_m;                /* A_m^3 */
    } else if (r == 1) {
        result = A_m1 * A_m * A_m;              /* A_m1 * A_m^2 */
    } else { /* r == 2 */
        result = A_m1 * A_m1 * A_m;             /* A_m1^2 * A_m */
    }

    return result;
}
