/*
 * A083916  Number of divisors of n that are congruent to 6 modulo 10.
 * https://oeis.org/A083916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n that are ≡ 6 (mod 10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses simple trial‑division factorisation and a
 * depth‑first search over the divisor tree, keeping only the
 * divisor modulo 10 at each step.  This is fast for the required
 * range (up to about 10⁴) and works with any integer type defined
 * by the surrounding build system. */

#include <stdint.h>
#include <stdbool.h>

/* Small static sieve to generate primes up to a fixed limit.
 * The limit 100000 is comfortably larger than needed for n ≤ 10⁵
 * and keeps the overhead negligible. */
static bool primes_done = false;
static int max_prime_limit = 100000;
static int primes[20000];
static int prime_count = 0;

static void init_prime_sieve(void)
{
    if (primes_done) return;
    bool *sieve = (bool *)calloc(max_prime_limit + 1, 1);
    for (int i = 2; i <= max_prime_limit; ++i) {
        if (!sieve[i]) {
            primes[prime_count++] = i;
            if ((int64_t)i * i <= max_prime_limit) {
                for (int j = i * i; j <= max_prime_limit; j += i)
                    sieve[j] = true;
            }
        }
    }
    free(sieve);
    primes_done = true;
}

/* Factorise n (as unsigned long long) into primes and exponents.
 * The arrays p[64], e[64] are filled and *cnt is set to the
 * number of distinct prime factors.  The caller must provide
 * space for at least 64 entries. */
static void factorise(unsigned long long n, int *p, int *e, int *cnt)
{
    *cnt = 0;
    for (int i = 0; i < prime_count && (unsigned long long)primes[i] * primes[i] <= n; ++i) {
        int pr = primes[i];
        if (n % pr == 0) {
            int exp = 0;
            do {
                ++exp;
                n /= pr;
            } while (n % pr == 0);
            p[(*cnt)]   = pr;
            e[(*cnt)]   = exp;
            ++(*cnt);
        }
    }
    if (n > 1) { /* remaining prime factor */
        p[(*cnt)]   = (int)n;
        e[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* Recursive helper: enumerate divisors using prime factors.
 * 'idx'     – current index in prime factor arrays
 * 'mod10'   – current divisor modulo 10
 * 'cnt[64]' – number of distinct prime factors
 * 'p[64]'   – prime values
 * 'e[64]'   – exponents
 * 'store'   – pointer to sequence type counter (seq_t) */
static void dfs_divisors(int idx, unsigned mod10,
                         const int *p, const int *e, int cnt,
                         seq_t *store)
{
    if (idx == cnt) {
        if (mod10 % 10 == 6)
            *store += 1;
        return;
    }
    int prime = p[idx];
    int exp   = e[idx];
    unsigned mod = mod10;
    // power of prime modulo 10
    unsigned pow_mod = 1 % 10;
    for (int k = 0; k <= exp; ++k) {
        dfs_divisors(idx + 1, (mod * pow_mod) % 10, p, e, cnt, store);
        pow_mod = (pow_mod * (prime % 10)) % 10;
    }
}

seq_t a(idx_t n)
{
    /* Ensure primes are ready */
    init_prime_sieve();

    /* Prepare for factorisation */
    unsigned long long val = (unsigned long long)n;
    int p[64], e[64], cnt;
    factorise(val, p, e, &cnt);

    seq_t cnt6 = 0;
    dfs_divisors(0, 1 % 10, p, e, cnt, &cnt6);
    return cnt6;
}
