/*
 * A072918  a(n) = p(n) - sigma(n), where p(n) is the least prime greater than
 *          sigma(n).
 * https://oeis.org/A072918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p(n) - sigma(n) where p(n) is the least prime strictly
 *            greater than sigma(n) (sum of divisors of n). The sequence is
 *            A072918. The implementation precomputes a sieve up to a fixed
 *            limit (200000) which easily covers all n up to about 10000.
 *            sigma(n) is computed from the smallest prime factor...
 * Range:     u8 n <= 386, u16 n <= 9636, u32 n <= 9636, u64 n <= 9636,
 *            u128 n <= 9636, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- parameters --------------------------------------- */
#define MAXN 200000      /* maximum number to sieve; enough for n<=10000 */
#define MAXP 20000       /* maximum number of primes in that range */

/* ---------- global tables ----------------------------------- */
static int spf[MAXN + 1];          /* smallest prime factor     */
static int primes[MAXP];           /* list of primes            */
static int prime_cnt = 0;          /* number of primes found    */
static int sieve_limit = 0;        /* highest number sieved     */

/* ---------- helper: sieve ------------------------------------ */
static void init_sieve(void)
{
    int i, j;
    for (i = 0; i <= MAXN; ++i) spf[i] = 0;
    spf[0] = 0;
    if (MAXN >= 1) spf[1] = 1;

    for (i = 2; i <= MAXN; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (prime_cnt < MAXP) primes[prime_cnt++] = i;
        }
        for (j = 0; j < prime_cnt; ++j) {
            int p = primes[j];
            long long prod = (long long)i * p;
            if (prod > MAXN) break;
            if (spf[prod] == 0) spf[prod] = p;
            if (p == spf[i]) break;
        }
    }
    sieve_limit = MAXN;
}

/* ---------- helper: is_prime -------------------------------- */
static int is_prime_uint64(uint64_t x)
{
    if (x < 2) return 0;
    if (x % 2 == 0) return (int)(x == 2);
    int i;
    for (i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        if ((uint64_t)p * (uint64_t)p > x) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* ---------- helper: next prime > n -------------------------- */
static uint64_t next_prime_gt(uint64_t start)
{
    uint64_t n = start;
    while (!is_prime_uint64(n)) ++n;
    return n;
}

/* ---------- helper: sigma(n) using spf --------------------- */
static seq_t sigma_of(idx_t n)
{
    seq_t result = 1;
    while (n > 1) {
        int p = spf[n];
        seq_t power = 1;
        seq_t sum   = 1;
        while (n % p == 0) {
            n /= p;
            power *= (seq_t)p;
            sum += power;
        }
        result *= sum;
    }
    return result;
}

/* ---------- main function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n>=1, but guard anyway */

    /* initialise sieve once */
    static int initialized = 0;
    if (!initialized) {
        init_sieve();
        initialized = 1;
    }

    /* compute sigma(n) */
    seq_t sigma = sigma_of((idx_t)n);

    /* find least prime > sigma */
    uint64_t prime = next_prime_gt((uint64_t)sigma + 1);

    /* result */
    return (seq_t)(prime - (uint64_t)sigma);
}
