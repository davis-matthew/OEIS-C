/*
 * A170401  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let a(n) be the number of reduced words of length n in the
 *            Coxeter group on 8 generators S_i with relations (S_i)^2 = (S_i
 *            S_j)^44 = I. The ordinary generating function is G(t) = (t^44 +
 *            2*t^43 + … + 2*t + 1) / (21*t^44 - 6*t^43 - 6*t^42 - … - 6*t +
 *            1). From this rational form the sequence satisf...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Buffer for the most recent 44 terms. */
    seq_t buf[44];
    /* Initial term a0. */
    buf[0] = (seq_t)1;
    /* Compute terms a1 … a43 using the first recurrence. */
    for (idx_t i = 1; i < 44; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= i; ++j)
            sum += buf[i - j];
        buf[i] = (seq_t)2 + (seq_t)6 * sum;
    }
    if (n < 44)
        return buf[n];

    /* Sum of a1 … a43 needed for a44 and later. */
    seq_t sum43 = (seq_t)0;
    for (idx_t i = 1; i < 44; ++i)
        sum43 += buf[i];

    /* Compute a44 using the special inhomogeneous relation. */
    seq_t a44 = (seq_t)1 + (seq_t)6 * sum43 - (seq_t)21 * buf[0] /* a0 */;
    if (n == 44)
        return a44;

    /* Store a44 in the buffer, overwriting a0. */
    buf[44 % 44] = a44;          /* buf[0] */
    /* Update the running sum of the last 43 values for the next step. */
    seq_t sumLast43 = sum43 + a44 - buf[1]; /* now sum of a2 … a44 */

    /* Compute a45 … a(n) iteratively. */
    for (idx_t i = 45; i <= n; ++i) {
        seq_t next = (seq_t)6 * sumLast43 - (seq_t)21 * buf[(i - 44) % 44];
        buf[i % 44] = next;
        sumLast43 += next;
        sumLast43 -= buf[(i - 43) % 44];   /* remove a_{i-43} from the window */
    }
    return buf[n % 44];
}
