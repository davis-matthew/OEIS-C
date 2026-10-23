/*
 * A015353  Gaussian binomial coefficient [ n,7 ] for q = -11.
 * https://oeis.org/A015353
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,7] for q = -11
 * Range:     i8 n <= 7, i16 n <= 7, i32 n <= 8, i64 n <= 9, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const int K = 7;          /* fixed k value */
    seq_t val[K + 1];         /* [cur_n, i] for i = 0..K */
    seq_t pow_q[K + 1];       /* q^(cur_n - i) for i = 1..K */
    seq_t q = (seq_t)-11;     /* base q */

    /* initial values */
    for (int i = 0; i <= K; ++i) {
        val[i] = 0;
        pow_q[i] = 0;
    }
    val[0] = 1;
    pow_q[1] = 1;             /* q^0 for n = 1 */

    /* iterate up to n */
    for (idx_t cur = 1; cur <= n; ++cur) {
        /* update values [cur, i] using the recurrence:
           [n, i] = [n-1, i] + q^(n-i) * [n-1, i-1]   */
        for (int i = K; i >= 1; --i) {
            if (i <= cur) {
                val[i] = val[i] + pow_q[i] * val[i - 1];
            }
        }

        /* update powers for the next n (cur + 1) */
        for (int i = K; i >= 1; --i) {
            if (i == cur + 1 && i <= K) {
                pow_q[i] = 1;          /* q^0 for new i = cur+1 */
            } else if (i <= cur && i <= K) {
                pow_q[i] = pow_q[i] * q; /* q^(cur+1 - i) */
            }
        }
    }

    return val[K];               /* [n, 7] */
}
