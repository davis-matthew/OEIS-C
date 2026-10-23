/*
 * A158613  Expansion of (1 - 2*x^3 - x^4 - x^5 + x^6 + x^7 - x^8)/(1 -
 *          x^3)^3.
 * https://oeis.org/A158613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy, less
 * Formula:   a(3k)=1, a(3k+1)=-(k), a(3k+2)=-(k^2)
 * Range:     i8 n <= 37, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute rest modulo 3 and the integer k = floor(n/3) */
    idx_t k_idx = n / 3;
    seq_t k = (seq_t)k_idx;

    idx_t r = n % 3;
    if (r == 0)                 /* a(3k) */
        return (seq_t)1;
    else if (r == 1)            /* a(3k+1) */
        return -k;
    else                        /* r == 2  -> a(3k+2) */
        return -(k * k);
}
