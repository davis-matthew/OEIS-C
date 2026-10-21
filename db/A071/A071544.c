/*
 * A071544  Smallest k such that n+k divides (k+1)!-k!.
 * https://oeis.org/A071544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k ≥ 1 such that n + k divides k! * k
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the smallest k for which (n + k) | k! * k.
   We simply test successive k, computing k! modulo (n + k) to avoid overflow. */

seq_t a(idx_t n)
{
    /* iterate k from 1 upwards */
    for (idx_t k = 1; ; ++k) {
        uint64_t mod = n + k;         /* n + k is always ≥ 2 for n ≥ 1 */
        uint64_t fact = 1 % mod;      /* current k! modulo mod */

        /* compute k! % mod */
        for (uint64_t i = 2; i <= k; ++i) {
            fact = (fact * i) % mod;
        }

        /* check if k! * k is divisible by (n + k) */
        uint64_t val = (fact * k) % mod;
        if (val == 0)
            return (seq_t)k;   /* the required minimal k */
    }
}
