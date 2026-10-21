/*
 * A203571  Period length 10: [0, 1, 2, 3, 4, 0, 4, 3, 2, 1] repeated.
 * https://oeis.org/A203571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod 5 if (-1)^floor(n/5) = +1 else -n mod 5, n >= 0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(n/5) and n mod 5 */
    idx_t q = n / 5;      /* quotient */
    idx_t r = n % 5;      /* remainder */

    /* If q is even: a(n) = r; if q is odd: a(n) = (5 - r) mod 5 */
    if ((q & 1) == 0) {          /* q even */
        return (seq_t)r;
    } else {                     /* q odd */
        if (r == 0) {            /* -0 mod 5 = 0 */
            return (seq_t)0;
        }
        return (seq_t)((seq_t)5 - (seq_t)r);
    }
}
