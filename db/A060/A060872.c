/*
 * A060872  Sum of d*d' over all unordered pairs (d,d') with d*d' = n.
 * https://oeis.org/A060872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * ceil( d(n) / 2 ) where d(n) is the number of divisors
 *            of n
 * Range:     u8 n <= 59, u16 n <= 3359, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count number of divisors d(n) */
    seq_t d = 1;
    idx_t m = n;

    /* Handle factor 2 */
    if (m % 2 == 0) {
        int exp = 0;
        do {
            m /= 2;
            exp++;
        } while (m % 2 == 0);
        d *= (exp + 1);
    }

    /* Handle odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            do {
                m /= p;
                exp++;
            } while (m % p == 0);
            d *= (exp + 1);
        }
    }

    /* Remaining prime factor > sqrt(n) */
    if (m > 1) {
        d *= 2;
    }

    /* Compute ceil(d/2) safely */
    seq_t half = (d + 1) / 2;

    /* Final result */
    return (seq_t)n * half;
}
