/*
 * A026214  a(n) = (1/2)*s(n), where s(n) is the n-th even number in A026177.
 * https://oeis.org/A026214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n even && lowest_nonzero_ternary_digit(n) == 2) ? n/2 :
 *            ceil(3n/2)
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is even and its lowest non‑zero ternary digit is 2,
       then a(n) = n/2; otherwise a(n) = ceil(3n/2). */
    if ((n & 1U) == 0) {
        idx_t m = n;
        while ((m % 3U) == 0U) m /= 3U;        /* strip trailing zeros in base 3 */
        if ((m % 3U) == 2U)                   /* lowest non‑zero ternary digit == 2 ? */
            return (seq_t)(n >> 1);            /* n/2 */
    }
    seq_t v = (seq_t)3 * (seq_t)n;             /* 3n in seq_t */
    return (v + 1) / 2;                       /* ceil(3n/2) = (3n+1)/2 for integers */
}
