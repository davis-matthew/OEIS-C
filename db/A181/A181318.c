/*
 * A181318  a(n) = A060819(n)^2.
 * https://oeis.org/A181318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = (n / gcd(n,4))^2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Quasi‑polynomial: a(4k) = k^2, a(4k+1) = (4k+1)^2, a(4k+2) = (2k+1)^2, a(4k+3) = (4k+3)^2 */

seq_t a(idx_t n)
{
    /* cast n to the result type (safe for the tested ranges) */
    seq_t nt = (seq_t)n;

    /* compute gcd(n,4) using only small integer logic */
    seq_t g;
    seq_t rem = nt & 3;                /* n % 4 for unsigned types */
    if (rem == 0)       g = 4;
    else if (rem == 2)  g = 2;
    else                g = 1;

    /* compute (n/g)^2 */
    seq_t base = nt / g;
    return base * base;
}
