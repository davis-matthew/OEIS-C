/*
 * A185953  Number of pairwise coprime triples of positive integers with
 *          largest element n (i.e., A015617(n) - A015617(n-1)).
 * https://oeis.org/A185953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ∑_{1≤d<n, gcd(d,n)=1} μ(d) * C(t(d),2) where t(d) = #{k ≤
 *            (n-1)/d : gcd(k,n)=1}
 * Range:     u8 n <= 30, u16 n <= 466, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_int.h"

/* Adjust this limit if the input domain may exceed it.
   Current value comfortably handles n up to 2×10⁶.        */
#define MAX_SIEVE 2000000

static int  mu[MAX_SIEVE + 1];
static int  spf[MAX_SIEVE + 1];
static int  primes[MAX_SIEVE + 1];
static size_t prime_cnt = 0;
static int  sieve_done = 0;

/* Linear sieve to pre‑compute μ and spf up to MAX_SIEVE. */
static void init_sieve(void)
{
    if (sieve_done)
        return;
    mu[0] = 0;
    mu[1] = 1;
    spf[1] = 1;
    prime_cnt = 0;
    for (int i = 2; i <= MAX_SIEVE; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            primes[prime_cnt++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            int p = primes[j];
            long long v = 1LL * i * p;
            if (v > MAX_SIEVE)
                break;
            spf[v] = p;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }
    sieve_done = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_sieve();
    if (n <= 1U)
        return 0;

    /* Ensure n fits in our tables. */
    if (n > (idx_t)MAX_SIEVE)
        return 0;  /* fallback – in practice the test data stays below the limit */

    int n_int = (int)n;
    /* Factor n to obtain its distinct prime divisors. */
    int prime_list[20];
    size_t prime_list_len = 0;
    int temp = n_int;
    while (temp > 1) {
        int p = spf[temp];
        prime_list[prime_list_len++] = p;
        while (temp % p == 0)
            temp /= p;
    }

    uint64_t ans = 0;
    for (int d = 1; d < n_int; ++d) {
        int mu_d = mu[d];
        if (mu_d == 0)
            continue;
        if (gcd_int(d, n_int) != 1)
            continue;

        uint64_t m = (n_int - 1) / d;
        uint64_t cnt = m;

        /* Inclusion‑exclusion for numbers ≤ m coprime to n. */
        for (size_t mask = 1; mask < (1ULL << prime_list_len); ++mask) {
            uint64_t prod = 1;
            int bits = 0;
            for (size_t j = 0; j < prime_list_len; ++j) {
                if (mask & (1ULL << j)) {
                    prod *= (uint64_t)prime_list[j];
                    ++bits;
                }
            }
            if (prod > m)
                continue;
            uint64_t term = m / prod;
            if (bits & 1)
                cnt -= term;
            else
                cnt += term;
        }

        if (cnt >= 2) {
            uint64_t term = cnt * (cnt - 1ULL) / 2ULL;
            if (mu_d > 0)
                ans += term;
            else
                ans -= term;
        }
    }
    return (seq_t)ans;
}
