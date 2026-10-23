/*
 * A028723  a(n) =
 *          (1/4)*floor(n/2)*floor((n-1)/2)*floor((n-2)/2)*floor((n-3)/2).
 * https://oeis.org/A028723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) =
 *            (1/4)*floor(n/2)*floor((n-1)/2)*floor((n-2)/2)*floor((n-3)/2)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 310, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a closed form that avoids fractional intermediate results. */
    seq_t nn = (seq_t)n;
    if ((nn & 1) == 0) {              /* n even:  n*(n-2)^2*(n-4)/64 */
        seq_t t1 = nn - 2;
        seq_t t2 = nn - 4;
        seq_t num = nn * t1 * t1 * t2;
        return num / 64;
    } else {                           /* n odd : (n-1)^2*(n-3)^2/64 */
        seq_t t1 = nn - 1;
        seq_t t2 = nn - 3;
        seq_t num = t1 * t1 * t2 * t2;
        return num / 64;
    }
}
