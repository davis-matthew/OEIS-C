/*
 * A061095  Number of ways of dividing n labeled items into labeled boxes with
 *          an equal number of items in each box.
 * https://oeis.org/A061095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{d|n} n! / ((n/d)!^d)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of ways to distribute n labeled items into labeled boxes
   with an equal number of items in each box. */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime generation (simple sieve) ---------- */
static bool *prime_sieve = NULL;
static int  primes[2000];
static int  prime_cnt = 0;
static int  prime_limit = 0;

static void ensure_primes(int limit)
{
    if (limit <= prime_limit) return;

    free(prime_sieve);
    prime_sieve = (bool*)malloc((limit + 1) * sizeof(bool));
    for (int i = 0; i <= limit; ++i) prime_sieve[i] = true;
    if (limit >= 0) prime_sieve[0] = false;
    if (limit >= 1) prime_sieve[1] = false;

    for (int i = 2; (long long)i * i <= limit; ++i) {
        if (prime_sieve[i]) {
            for (int j = i * i; j <= limit; j += i) prime_sieve[j] = false;
        }
    }

    prime_cnt = 0;
    for (int i = 2; i <= limit; ++i) if (prime_sieve[i]) primes[prime_cnt++] = i;
    prime_limit = limit;
}

/* ---------- Legendre exponent of prime p in n! ---------- */
static int legendre(int n, int p)
{
    int e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* ---------- Factor n into primes (primes and exponents stored in arrays) ---------- */
static void factor_n(int n, int *pr, int *exp, int *cnt)
{
    *cnt = 0;
    int temp = n;
    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        if ((long long)p * p > temp) break;
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            pr[(*cnt)] = p;
            exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (temp > 1) {
        pr[(*cnt)]    = temp;
        exp[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* ---------- Recursive generation of all divisors of n ---------- */
static void gen_divisors_rec(int idx, long long cur,
                             const int *pr, const int *exp, int cnt,
                             long long *divs, int *divCnt)
{
    if (idx == cnt) {
        divs[(*divCnt)++] = cur;
        return;
    }
    long long val = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divisors_rec(idx + 1, val, pr, exp, cnt, divs, divCnt);
        val *= pr[idx];
    }
}

static void get_divisors(int n, long long *divs, int *divCnt)
{
    int pr[20], exp[20], cnt;
    factor_n(n, pr, exp, &cnt);
    *divCnt = 0;
    gen_divisors_rec(0, 1LL, pr, exp, cnt, divs, divCnt);
}

/* ---------- Fast exponentiation for seq_t ---------- */
static seq_t pow_seq_t(seq_t base, int exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Ensure primes up to n are available */
    ensure_primes((int)n);

    /* Get all divisors of n */
    long long divs[2000];
    int divCnt;
    get_divisors((int)n, divs, &divCnt);

    seq_t sum = 0;
    for (int i = 0; i < divCnt; ++i) {
        int d = (int)divs[i];         // divisor
        int m = (int)(n / d);         // items per box

        seq_t term = 1;

        /* For each prime, compute exponent in the term */
        for (int pi = 0; pi < prime_cnt && primes[pi] <= (int)n; ++pi) {
            int p = primes[pi];
            int e_n = legendre((int)n, p);
            int e_m = legendre(m, p);
            int e = e_n - d * e_m;
            if (e > 0) {
                term *= pow_seq_t((seq_t)p, e);
            }
        }

        sum += term;
    }
    return sum;
}
