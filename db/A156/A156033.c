/*
 * A156033  Numerator of (Sum_{k=1..n} k^3)/n!.
 * https://oeis.org/A156033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (∑_{k=1}^{n} k^3) / n! = ((n(n+1)/2)^2) /
 *            gcd(((n(n+1)/2)^2), n!)
 * Range:     u8 n <= 15, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define PRIME_LIMIT 10001      /* covers n+1 for n ≤ 10000 */
#define MAX_PRIMES 2000        /* sufficient for primes up to 10001 */

static int  primes[MAX_PRIMES];
static size_t prime_count = 0;
static int  primes_inited = 0;

/* Simple sieve to fill the primes array */
static void init_primes(void)
{
    if (primes_inited) return;
    static int sieve[PRIME_LIMIT + 1];
    for (int i = 2; i <= PRIME_LIMIT; ++i) sieve[i] = 1;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes[prime_count++] = i;
    primes_inited = 1;
}

/* Legendre exponent of p in n! */
static inline idx_t exponent_in_factorial(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* Integer power: base^exp (exp ≥ 0) */
static inline seq_t pow_seq_t(seq_t base, int exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Process the prime factorisation of x: multiply numerator by
   the part of each prime power that is not cancelled by n!  */
static void factor_and_process(idx_t x, seq_t *numerator, idx_t n)
{
    for (size_t i = 0; i < prime_count && (idx_t)primes[i] * (idx_t)primes[i] <= x; ++i) {
        idx_t p = primes[i];
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            int e_S = 2 * e;                     /* exponent in (n(n+1)/2)^2 */
            idx_t e_nf = exponent_in_factorial(n, p);
            if ((idx_t)e_S > e_nf)
                *numerator *= pow_seq_t((seq_t)p, e_S - (int)e_nf);
        }
    }
    if (x > 1) { /* remaining prime factor */
        idx_t p = x;
        int e_S = 2;
        idx_t e_nf = exponent_in_factorial(n, p);
        if ((idx_t)e_S > e_nf)
            *numerator *= pow_seq_t((seq_t)p, e_S - (int)e_nf);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    init_primes();

    seq_t numerator = 1;

    idx_t a = n;
    idx_t b = n + 1;
    if (a % 2 == 0) a /= 2;          /* divide the even one by 2 */
    else b /= 2;

    /* n and n+1 are coprime; factorisation can be done independently */
    factor_and_process(a, &numerator, n);
    factor_and_process(b, &numerator, n);

    return numerator;
}
