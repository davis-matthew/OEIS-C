/*
 * A191484  Number of compositions of even natural numbers into 5 parts <= n.
 * https://oeis.org/A191484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 1)^5 + (1 + (-1)^n)/2)/2
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 95, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t and compute (n+1)^5 */
    seq_t t = (seq_t)(n + 1);
    t *= t;     /* t = (n+1)^2 */
    t *= (seq_t)(n + 1); /* t = (n+1)^3 */
    t *= (seq_t)(n + 1); /* t = (n+1)^4 */
    t *= (seq_t)(n + 1); /* t = (n+1)^5 */

    /* Add parity adjustment: +1 if n is even, else +0 */
    if ((n & 1) == 0) {
        t += 1;
    }

    /* Divide by 2 (integer division) */
    return t / 2;
}
