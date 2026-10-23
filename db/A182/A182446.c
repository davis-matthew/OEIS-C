/*
 * A182446  a(n) = Sum_{k = 0..n} C(n,k)^9.
 * https://oeis.org/A182446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182446: a(n) = Sum_{k = 0..n} C(n,k)^9
   (offset 0) */
#include <stdint.h>
#include <stddef.h>

/* Compute c^9 by repeated multiplication.  This keeps the code
   independent from the (optional) big_pow utility.  */
static inline seq_t pow9(seq_t c) {
    seq_t r = c;
    /* multiply r by c eight more times to achieve c^9 */
    for (int i = 0; i < 8; ++i) {
        r *= c;
    }
    return r;
}

/* a(n) = \sum_{k=0}^n binomial(n,k)^9   */
seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t c = 1;              /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        /* add C(n,k)^9 to sum */
        sum += pow9(c);

        /* update c to C(n,k+1) for next iteration, except
           when k == n, where there is no next term. */
        if (k == n) {
            break;
        }
        /* C(n,k+1) = C(n,k) * (n - k) / (k + 1) */
        c = (c * (idx_t)(n - k)) / (idx_t)(k + 1);
    }
    return sum;
}
