/*
 * A040923  Continued fraction for sqrt(954).
 * https://oeis.org/A040923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 30 (n=0); for n>=1 a(n) = period[(n-1)%14] where period =
 *            {1,7,1,5,3,2,6,2,3,5,1,7,1,60} (the continued‑fraction
 *            coefficients of sqrt(954))
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* period of the continued fraction for sqrt(954) after the
       initial term 30; period length is 14. */
    static const seq_t period[14] = {
        1, 7, 1, 5, 3, 2,
        6, 2, 3, 5, 1, 7,
        1, 60
    };

    if (n == 0) {
        return 30;
    }

    idx_t i = (n - 1) % 14;
    return period[i];
}
