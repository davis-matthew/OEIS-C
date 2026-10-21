/*
 * A098035  a(n) = Sum_{d|n} mu(d+1), where mu() is the Moebius function.
 * https://oeis.org/A098035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} mu(d+1) (mu = Möbius function)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The implementation below computes a(n) by iterating over all divisors
 * d of n, evaluating the Möbius value of (d+1) and accumulating the sum.
 * The Möbius function is calculated by trial division using a small
 * prime sieve up to 100000.  All arithmetic is performed using the
 * provided integer types `idx_t` (unsigned 64‑bit) and `seq_t`.
 */

#include <stdint.h>

/* Simple static sieve for primes up to 100000 */
static int primes[20000];
static int prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt > 0) return;           /* already initialized */
    const int LIMIT = 100000;
    int sieve[LIMIT+1];
    int i, j;
    for (i = 0; i <= LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (i = 2; i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (j = i * i; j <= LIMIT; j += i) sieve[j] = 0;
        }
    }
    for (i = 2; i <= LIMIT; ++i) if (sieve[i]) primes[prime_cnt++] = i;
}

/* Möbius function for uint64_t: μ(n) = 0 if n has a squared prime factor,
   otherwise (-1)^k where k = number of prime factors. */
static int mu_val(uint64_t x)
{
    init_primes();
    int cnt = 0;
    int i;
    for (i = 0; i < prime_cnt && (uint64_t)primes[i]*primes[i] <= x; ++i) {
        int p = primes[i];
        if (x % p == 0) {
            x /= p;
            if (x % p == 0) return 0;   /* square factor */
            cnt++;
        }
    }
    if (x > 1) cnt++;   /* remaining prime factor */
    return (cnt & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t d;
    for (d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            sum += (seq_t)mu_val(d + 1);
            idx_t e = n / d;
            if (e != d) {
                /* complementary divisor e */
                sum += (seq_t)mu_val(e + 1);
            }
        }
    }
    return sum;
}
