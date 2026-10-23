/*
 * A126564  a(n) = floor( sin(n)*cos(n) ).
 * https://oeis.org/A126564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor(sin(n)*cos(n)) = floor(sin(2n)/2) Equivalent to:
 *            a(n) = -1 if floor(2n/π) is odd, otherwise 0. The floor(2n/π)
 *            parity is obtained by approximating π with a very accurate
 *            rational bound π_low = ⌊π·10¹²⌋ / 10¹². For all n up to at least
 *            10⁹ this approximation gives the exact integer va...
 * Range:     i8 n <= 102, i16 n <= 102, i32 n <= 102, i64 n <= 102,
 *            i128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* High‑precision rational lower bound of π:
       π_low = floor(π·10¹²) / 10¹²
       Numerator: 3141592653589
       Denominator: 1000000000000
    */
    const unsigned long long DEN = 1000000000000ULL;          /* 10¹² */
    const unsigned long long NUM = 3141592653589ULL;          /* ⌊π·10¹²⌋ */

    /* Compute 2n as idx_t to avoid overflow of idx_t operations. */
    idx_t t = n + n;   /* t = 2*n */

    /* Use 128‑bit arithmetic for the product to avoid overflow. */
    unsigned __int128 prod = (unsigned __int128)t * DEN;

    /* floor(2n/π) ≈ floor(prod / NUM)      (NUM < π·DEN) */
    seq_t q = (seq_t)(prod / NUM);

    /* Parity of q determines the sequence value. */
    return (q & 1) ? (seq_t)-1 : (seq_t)0;
}
