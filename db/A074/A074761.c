/*
 * A074761  Number of partitions of n of order n.
 * https://oeis.org/A074761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Number of partitions of n of order n
 * Range:     u8 n <= 29, u16 n <= 83, u32 n <= 239, u64 n <= 839,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) equals the number of conjugacy classes of S_n having an element of
   order n.  This implementation uses inclusion–exclusion over the prime
   factors of n and classic partition DP.

   For a given n, let divisors(n) be the set of all positive divisors of n.
   For each subset T of the prime factors of n, let D_T be the subset of
   divisors of n that are NOT divisible by the maximal prime‑power for any
   prime in T.  The number of partitions of n using only parts from D_T
   is computed with a standard DP.  Inclusion–exclusion over T yields
   a(n).                                                    */

/* Standard headers -------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>

/* ----------------------------------------------------------------------------
   Function to compute the number of partitions of n of order n.
   Seq_t and idx_t are supplied by the build (see problem statement).
   ----------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – undefined for 0 */

    /* ---- compute all divisors of n -------------------------------------*/
    int max_divs = 1;          /* at least one divisor (n itself) */
    for (idx_t i = 1; i * i <= n; ++i)
        if (n % i == 0) {
            if (i * i == n) max_divs += 1;
            else             max_divs += 2;
        }
    uint64_t *divs = (uint64_t *)malloc(max_divs * sizeof(uint64_t));
    int m = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[m++] = i;
            if (i * i != n) divs[m++] = n / i;
        }
    }
    /* sort divisors ascending (small size -> simple bubble sort) */
    for (int i = 0; i < m - 1; ++i)
        for (int j = i + 1; j < m; ++j)
            if (divs[i] > divs[j]) {
                uint64_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }

    /* ---- factor n into primes ------------------------------------------*/
    uint64_t primes[12];
    uint64_t prime_pw[12];
    int k = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t pw = 1;
            do {
                temp /= p;
                pw *= p;
            } while (temp % p == 0);
            primes[k] = p;
            prime_pw[k] = pw;
            ++k;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        primes[k] = temp;
        prime_pw[k] = temp;
        ++k;
    }

    /* ---- DP array -------------------------------------------------------*/
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* ---- iterate over all subsets of prime factors ---------------------*/
    int total_masks = 1 << k;
    for (int mask = 0; mask < total_masks; ++mask) {
        /* reset dp array to zero, set base case */
        for (idx_t s = 0; s <= n; ++s) dp[s] = 0;
        dp[0] = 1;

        /* DP over allowed parts (divisors that avoid the prime powers in mask) */
        for (int di = 0; di < m; ++di) {
            uint64_t d = divs[di];
            int allowed = 1;
            for (int pj = 0; pj < k; ++pj)
                if (mask & (1 << pj)) {
                    if (d % prime_pw[pj] == 0) {
                        allowed = 0;
                        break;
                    }
                }
            if (!allowed) continue;

            /* unbounded knapsack for part size d */
            idx_t part = d;
            for (idx_t s = part; s <= n; ++s) {
                dp[s] = dp[s] + dp[s - part];
            }
        }

        /* accumulate with inclusion–exclusion */
        if (__builtin_popcount((unsigned)mask) & 1)
            sum_neg = sum_neg + dp[n];
        else
            sum_pos = sum_pos + dp[n];
    }

    /* final result: sum_pos - sum_neg (should be non‑negative) */
    seq_t result = sum_pos - sum_neg;

    /* free memory */
    free(dp);
    free(divs);

    return result;
}
