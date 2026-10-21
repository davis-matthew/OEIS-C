/*
 * A065642  a(1) = 1; for n > 1, a(n) = Min {m > n | m has same prime factors
 *          as n ignoring multiplicity}.
 * https://oeis.org/A065642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 8810, u64 n <= 8810,
 *            u128 n <= 8810, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A065642
    a(1) = 1
    for n > 1, a(n) = the smallest integer m>n such that m has exactly
    the same set of distinct prime factors as n.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define SIEVE_MAX 100000   /* enough for √(max candidate) with n≤10000 */

/* ----------  prime sieve (static, one‑time)  ---------- */
static int primes[SIEVE_MAX + 1];
static int prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    bool isp[SIEVE_MAX + 1];
    for (int i = 2; i <= SIEVE_MAX; ++i) isp[i] = true;
    for (int i = 2; i * i <= SIEVE_MAX; ++i) {
        if (isp[i]) {
            for (int j = i * i; j <= SIEVE_MAX; j += i) isp[j] = false;
        }
    }
    for (int i = 2; i <= SIEVE_MAX; ++i) if (isp[i]) primes[prime_cnt++] = i;
    primes_initialized = true;
}

/* ----------  factor n into distinct primes and radical  ---------- */
static void factor_rad(idx_t n, seq_t *rad, int *rad_primes, int *rad_cnt)
{
    init_primes();
    seq_t r = 1;
    idx_t m = n;
    int cnt = 0;
    for (int i = 0; i < prime_cnt && (seq_t)primes[i] * (seq_t)primes[i] <= m; ++i) {
        int p = primes[i];
        if (m % p == 0) {
            rad_primes[cnt++] = p;
            r *= p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {                  /* remaining prime factor > sqrt(n) */
        rad_primes[cnt++] = (int)m;
        r *= m;
    }
    *rad = r;
    *rad_cnt = cnt;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 1) return 1;

    int rad_primes[20];
    int rad_cnt;
    seq_t rad_value;
    factor_rad(n, &rad_value, rad_primes, &rad_cnt);

    /* candidates must be multiples of rad_value */
    seq_t candidate = (n / rad_value + 1) * rad_value;
    for (;; candidate += rad_value) {
        seq_t t = candidate;
        for (int i = 0; i < rad_cnt; ++i) {
            int p = rad_primes[i];
            while (t % p == 0) t /= p;
        }
        if (t == 1) return candidate;   /* same prime set */
    }
}
