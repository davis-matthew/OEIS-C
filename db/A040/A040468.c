/*
 * A040468  Continued fraction for sqrt(491).
 * https://oeis.org/A040468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the simple continued fraction expansion of sqrt(491).
   a(0) = floor(sqrt(491)); subsequent terms repeat in a period
   of length 22: 6,3,4,8,1,1,1,2,1,1,21,1,1,2,1,1,1,8,4,3,6,44.
   The function recomputes the period for each call. */
seq_t a(idx_t n)
{
    /* Constant for the continued fraction: D = 491 */
    static const seq_t D = 491;

    /* Compute a0 = floor(sqrt(D)) using a simple integer method. */
    seq_t a0 = 0;
    seq_t r = 0;
    while ((r + 1) * (r + 1) <= D) {
        ++r;
    }
    a0 = r;

    if (n == 0) {
        return a0;
    }

    /* Compute the period of the continued fraction. */
    seq_t period[256];
    int pos = 0;
    seq_t m = 0, d = 1, a = a0;

    while (1) {
        seq_t m2 = d * a - m;
        seq_t d2 = (D - m2 * m2) / d;
        seq_t a2 = (a0 + m2) / d2;
        period[pos++] = a2;
        if (a2 == 2 * a0) {
            break;
        }
        m = m2;
        d = d2;
        a = a2;
    }

    idx_t rem = (n - 1) % (idx_t)pos;
    return period[rem];
}
