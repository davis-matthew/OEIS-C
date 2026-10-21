/*
 * A068792  a(n) = (n-1)*n^(n-2) + Sum_{i=1..n} (n-i)*(n^(n-i-1) + n^(n+i-3)).
 * https://oeis.org/A068792
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n^(n-1) - 1)/(n-1))^2 (for n >= 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* undefined for n < 2 */

    /* Compute n^(n-1) using binary exponentiation */
    seq_t power = (seq_t)n;
    idx_t e = n - 1;
    seq_t result = 1;
    while (e > 0) {
        if (e & 1) result *= power;
        power *= power;
        e >>= 1;
    }
    /* result now holds n^(n-1) in seq_t */

    /* (n^(n-1) - 1)/(n-1) */
    seq_t div = (result - 1u) / (seq_t)(n - 1);

    /* Final answer: square */
    return div * div;
}
