/*
 * A016002  a(n) = (tau(n^7)+6)/7.
 * https://oeis.org/A016002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(n^7)+6)/7
 * Range:     u8 n <= 29, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */
    idx_t m = n;
    seq_t prod = 1;

    /* factor 2 separately */
    idx_t exp = 0;
    while ((m & 1) == 0) {           /* m is even */
        exp++;
        m >>= 1;                     /* divide by 2 */
    }
    if (exp) {
        prod *= ((seq_t)7 * exp + 1);
    }

    /* odd factors */
    for (idx_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            exp = 0;
            while (m % i == 0) {
                exp++;
                m /= i;
            }
            prod *= ((seq_t)7 * exp + 1);
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        prod *= ((seq_t)7 * 1 + 1);   /* exponent is 1 */
    }

    return (prod + 6) / 7;
}
