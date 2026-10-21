/*
 * A091304  a(n) = Omega(2n-1) (number of prime factors of the n-th odd
 *          number, counted with multiplicity).
 * https://oeis.org/A091304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Omega(2n-1) (number of prime factors of 2n‑1, counted
 *            with multiplicity)
 * Range:     u8 n <= 853, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the odd integer 2n‑1 */
    seq_t m = (seq_t)n * 2 - 1;
    seq_t cnt = 0;
    seq_t d  = 3;          /* start trial division from 3 (2 is impossible, m is odd) */
    /* iterate while d² ≤ m, using unsigned long long to avoid overflow for small seq_t types */
    while ((unsigned long long)d * d <= (unsigned long long)m)
    {
        while (m % d == 0)
        {
            m /= d;
            cnt++;
        }
        d += 2;            /* only test odd divisors */
    }
    /* if a prime factor larger than sqrt remains, it appears exactly once */
    if (m > 1)
        cnt++;
    return cnt;
}
