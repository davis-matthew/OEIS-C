/*
 * A082863  Number of distinct prime factors of n^2-1.
 * https://oeis.org/A082863
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221((n-1)*(n+1))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. count the distinct prime factors of (n-1)*(n+1) */

/* Count distinct prime factors of an integer using simple trial division. */
static seq_t count_distinct_primes(idx_t x)
{
    seq_t cnt = 0;

    if (x <= 1)
        return 0;

    /* factor 2 */
    if ((x & 1U) == 0) {
        cnt++;
        while ((x & 1U) == 0)
            x >>= 1;
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            cnt++;
            while (x % p == 0)
                x /= p;
        }
    }

    if (x > 1)
        cnt++;

    return cnt;
}

seq_t a(idx_t n)
{
    /* n >= 2 (offset 2); the function works for any n >= 2           */
    seq_t cnt1 = count_distinct_primes(n - 1);
    seq_t cnt2 = count_distinct_primes(n + 1);
    seq_t res  = cnt1 + cnt2;

    /* (n-1) and (n+1) share the prime 2 iff n is odd              */
    if (n & 1U)
        res -= (seq_t)1;

    return res;
}
