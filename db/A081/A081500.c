/*
 * A081500  In a triangle the n-th row begins with n and contains n-1 smallest
 *          numbers coprime to n and greater than n. Sequence gives the
 *          leading diagonal.
 * https://oeis.org/A081500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = n + t, where t is the smallest integer such that ψ(t) =
 *            n-1 and ψ(t) = |{m ≤ t : gcd(m, n) = 1}|. This t is the (n-1)-th
 *            smallest number > n that is coprime to n. The result a(n) is the
 *            last element of the n-th row of the triangle. The algorithm
 *            factors n, counts coprime numbers up to a va...
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9871 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count coprime integers m (1 ≤ m ≤ t) with respect to n, given the
   distinct prime factors of n in array 'pr' of length 'cnt'. */
static uint64_t count_coprime_upto(uint64_t t,
                                   const uint64_t *pr,
                                   int cnt)
{
    if (t == 0) return 0;
    uint64_t not_coprime = 0;
    int subsets = 1 << cnt;
    for (int mask = 1; mask < subsets; ++mask) {
        uint64_t prod = 1;
        int bits = 0;
        for (int i = 0; i < cnt; ++i) {
            if (mask & (1 << i)) {
                prod *= pr[i];
                ++bits;
                if (prod > t) break;
            }
        }
        if (prod > t) continue;
        uint64_t term = t / prod;
        if (bits & 1) not_coprime += term;
        else          not_coprime -= term;
    }
    return t - not_coprime;
}

/* Factor n into its distinct prime factors (max 6 for n ≤ 10000).
   The function returns the number of distinct primes, and stores
   them in the array 'pr'. */
static int factor_distinct_primes(uint64_t n, uint64_t *pr)
{
    int cnt = 0;
    if (n % 2 == 0) {
        pr[cnt++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            pr[cnt++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) pr[cnt++] = n;
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    uint64_t N = (uint64_t)n;
    uint64_t primes[6];
    int prime_cnt = factor_distinct_primes(N, primes);

    /* ----------------------------------------------------------
       Binary search for minimal t such that
           count_coprime_upto(t, primes, prime_cnt) >= N-1
       ---------------------------------------------------------- */
    uint64_t low = 0;
    uint64_t high = N;
    /* Ensure high is large enough */
    while (count_coprime_upto(high, primes, prime_cnt) < (N - 1))
        high <<= 1;                        /* double until sufficient */

    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t cnt = count_coprime_upto(mid, primes, prime_cnt);
        if (cnt >= (N - 1))
            high = mid;
        else
            low = mid + 1;
    }

    /* low (== high) is the smallest t */
    return (seq_t)(N + low);
}
