/*
 * A104257  Square array T(a,n) read by antidiagonals: replace 2^i with a^i in
 *          binary representation of n, where a,n >= 2.
 * https://oeis.org/A104257
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(a, n2), where (a, n2) is obtained by enumerating the *
 *            points of the square array T(a, n) by antidiagonals. * * Let
 *            T(a, 2k) = a * T(a, k), * Let T(a, 2k+1) = a * T(a, k) + 1. * *
 *            This code translates the sequence index to the corresponding *
 *            pair (a, n2) and evaluates T(a, n2)
 * Range:     u8 n <= 73, u16 n <= 421, u32 n <= 5214, u64 n <= 9731,
 *            u128 n <= 9731, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;               /* sequence is defined for n ≥ 2 */
    idx_t k = n - 2;                   /* zero‑based position in the array */

    /* Find the diagonal t (s = t + 3) such that
     * t(t+1)/2 ≥ k + 1.  t is the number of terms on that diagonal. */
    idx_t target = k + 1;              /* when t = 1 we already have 1 term */
    double discrim = sqrt(1.0 + 8.0 * (double)target);
    idx_t t = (idx_t)((discrim - 1.0) / 2.0); /* floor of the root */
    if ((t * (t + 1)) / 2 < target) t++;       /* correct to ceil */

    /* Number of terms before this diagonal */
    idx_t prev = (t - 1) * t / 2;

    /* Position (offset) within the diagonal, 0‑based */
    idx_t offset = k - prev;           /* 0 ≤ offset < t */

    /* Column index n2 for this position (n2 grows with offset) */
    idx_t n2 = 2 + offset;

    /* Row index a = (t + 3) - n2 = (t + 1) - offset */
    seq_t a_val = (seq_t)(t + 1 - offset);

    /* Evaluate T(a_val, n2) by expanding the binary digits of n2 */
    seq_t result = 0;
    seq_t power   = 1;                 /* a_val^0 initially */
    idx_t temp = n2;
    while (temp) {
        if (temp & 1) result += power; /* add a_val^bit_position if bit is 1 */
        power *= a_val;
        temp >>= 1;
    }
    return result;
}
