/*
 * A123330  Expansion of eta(q^2) * eta(q^3)^6 / (eta(q)^2 * eta(q^6)^3) in
 *          powers of q.
 * https://oeis.org/A123330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=2*b(n) where b is multiplicative with
 *            b(2^e)=(1-3*(-1)^e)/2, b(3^e)=1, b(p^e)=e+1 if p≡1 (mod 6),
 *            b(p^e)=(1+(-1)^e)/2 if p≡5 (mod 6)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int primes_inited = 0;
static const unsigned int PRIME_MAX = 1000000U;
static size_t prime_cnt = 0;
static uint32_t primes[78500];  /* number of primes <= 1e6 ≈ 78498 */

/* Simple sieve to fill the primes array */
static void init_primes(void)
{
    if (primes_inited) return;
    /* cast malloc for C++ compatibility */
    char *is_prime = (char *)malloc(PRIME_MAX + 1);
    if (!is_prime) exit(EXIT_FAILURE);
    for (unsigned int i = 0; i <= PRIME_MAX; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (unsigned int i = 2; i * i <= PRIME_MAX; ++i) {
        if (is_prime[i]) {
            for (unsigned int j = i * i; j <= PRIME_MAX; j += i) {
                is_prime[j] = 0;
            }
        }
    }
    for (unsigned int i = 2; i <= PRIME_MAX; ++i) {
        if (is_prime[i]) primes[prime_cnt++] = i;
    }
    free(is_prime);
    primes_inited = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* offset 0 */

    init_primes();

    idx_t m = n;
    seq_t val = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > m) break;
        if (m % p) continue;
        idx_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (p == 2) {
            if (exp & 1) val *= (seq_t)2;
            /* even exponents contribute 1 (no multiplication) */
        } else if (p == 3) {
            /* multiplicative unit, nothing to do */
        } else if (p % 6 == 1) {
            val *= (seq_t)(exp + 1);
        } else if (p % 6 == 5) {
            if (exp & 1) return (seq_t)0;   /* odd exponent -> zero */
            /* even exponents contribute 1 (no multiplication) */
        }
    }
    if (m > 1) { /* remaining prime factor > sqrt(original n) */
        uint32_t p = (uint32_t)m;
        if (p == 2) {
            val *= (seq_t)2; /* exponent 1 is odd */
        } else if (p == 3) {
            /* nothing */
        } else if (p % 6 == 1) {
            val *= (seq_t)2; /* exp = 1 → (exp+1)=2 */
        } else if (p % 6 == 5) {
            return (seq_t)0; /* odd exponent */
        }
    }

    return val * (seq_t)2;   /* a(n) = 2*|b(n)| */
}
