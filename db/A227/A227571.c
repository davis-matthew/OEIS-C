/*
 * A227571  Denominators of rationals with e.g.f. D(3,x), a Debye function.
 * https://oeis.org/A227571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator(3*B(n)/(n+3))
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* B(n) is the nth Bernoulli number.  For odd n>1, B(n)=0, so a(n)=1.  
   For n=1, B(1)=-1/2, giving a(1)=8.  For even n=2m, the denominator of
   B(2m) is Π{p | (p-1) | 2m} p.  Since 3 divides that product, the
   denominator of r(3,n) = 3*B(n)/(n+3) equals
        (product/3) * (n+3).                          */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Maximum n expected (≈10000).  The sieve is built up to n+1.           */
#define MAX_PRIME 20001                 /* primes up to this make all needed cases */

/* Storage for the prime list produced by the sieve. */
static uint32_t prime_list[3000];
static size_t   prime_count = 0;
static bool     sieve_done  = false;

/* Simple sieve to generate all primes ≤ MAX_PRIME.                       */
static void init_sieve(void)
{
    static bool is_prime[MAX_PRIME + 1];
    /* initialise */
    for (int i = 2; i <= MAX_PRIME; ++i) is_prime[i] = true;

    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i)
                is_prime[j] = false;
        }
    }

    /* collect primes */
    prime_count = 0;
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (is_prime[i]) {
            prime_list[prime_count++] = (uint32_t)i;
        }
    }
    sieve_done = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* special cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n & 1U) return (seq_t)1;   /* odd n > 1 */

    /* ensure primes are ready */
    if (!sieve_done) init_sieve();

    seq_t prod = (seq_t)1;

    /* product of primes p with (p-1) | n, where n is even. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if (p > (uint32_t)(n + 1)) break;          /* p-1 > n → stop */
        if (n % ((idx_t)p - 1U) == 0) {
            prod *= (seq_t)p;
        }
    }

    /* prod is the denominator of B(n).  Divide by 3 and multiply by (n+3). */
    seq_t denom = prod / (seq_t)3U;        /* guaranteed integer division */
    denom *= (seq_t)(n + 3U);

    return denom;
}
