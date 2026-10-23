/*
 * A046667  a(n) = A046666(n)/2.
 * https://oeis.org/A046667
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - spf(n)) / 2 where spf(n) is the smallest prime
 *            factor of n
 * Range:     u8 n <= 263, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)   /* behaviour below the offset is irrelevant */
        return 0;

    /* compute the smallest prime factor of n */
    uint64_t spf = 0;
    if ((n & 1) == 0) {                /* n is even */
        spf = 2;
    } else {
        for (idx_t i = 3; i <= n / i; i += 2) {
            if (n % i == 0) {
                spf = i;
                break;
            }
        }
        if (spf == 0)   /* n is odd prime */
            spf = (uint64_t)n;
    }

    /* compute the term, using seq_t arithmetic */
    seq_t result = ((seq_t)(n - spf)) / 2;
    return result;
}
