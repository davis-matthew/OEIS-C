/*
 * A053214  Central binomial coefficients (A000984) read mod 2n, with a(0)=1.
 * https://oeis.org/A053214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2·n, n) mod (2·n) (with a(0) = 1)
 * Range:     u8 n <= 160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* -------------------------------------------------- */
/* Modular multiplication using 64‑bit intermediates
   (works even when seq_t is smaller than 64 bits) */
static seq_t mul_mod(seq_t a, seq_t b, seq_t mod) {
    unsigned long long M = (unsigned long long)mod;
    unsigned long long res = 0;
    unsigned long long A = (unsigned long long)a % M;
    unsigned long long B = (unsigned long long)b;
    while (B) {
        if (B & 1ULL) res = (res + A) % M;
        A = (A + A) % M;
        B >>= 1ULL;
    }
    return (seq_t)res;
}

/* Modular exponentiation: base^exp mod mod */
static seq_t mod_pow(seq_t base, unsigned long long exp, seq_t mod) {
    unsigned long long M = (unsigned long long)mod;
    unsigned long long res = 1 % M;
    unsigned long long B = (unsigned long long)base % M;
    while (exp) {
        if (exp & 1ULL) res = (res * B) % M;
        B = (B * B) % M;
        exp >>= 1ULL;
    }
    return (seq_t)res;
}

/* -------------------------------------------------- */
/* Sieve of Eratosthenes up to LIMIT = 20000 (covers n ≤ 10000) */
#define LIMIT 20000

static unsigned char is_composite[LIMIT + 1] = {0};
static uint32_t primes_buf[LIMIT + 1];
static size_t prime_count = 0;

static void init_sieve(void) {
    if (prime_count != 0) return;           /* already initialized */
    for (size_t p = 2; p <= LIMIT; ++p) {
        if (!is_composite[p]) {
            primes_buf[prime_count++] = (uint32_t)p;
            if (p * p <= LIMIT) {
                for (size_t j = p * p; j <= LIMIT; j += p)
                    is_composite[j] = 1;
            }
        }
    }
}

/* -------------------------------------------------- */
/* Legendre's formula: exponent of prime p in n! */
static unsigned long long legendre_exp(unsigned long long n, unsigned long long p) {
    unsigned long long e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* -------------------------------------------------- */
/* Sequence function */
seq_t a(idx_t n) {
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    init_sieve();

    seq_t mod = (seq_t)(2ULL * (unsigned long long)n);     /* modulus 2·n (n>0) */
    unsigned long long two_n = 2ULL * (unsigned long long)n;
    unsigned long long result = 1 % (unsigned long long)mod;  /* use 64-bit for safety */

    /* multiply contributions from each prime ≤ 2n */
    for (size_t i = 0; i < prime_count && primes_buf[i] <= two_n; ++i) {
        unsigned long long p = (unsigned long long)primes_buf[i];
        unsigned long long e = legendre_exp(two_n, p) -
                                2ULL * legendre_exp((unsigned long long)n, p);
        if (!e) continue;
        unsigned long long pp = (unsigned long long)mod_pow((seq_t)p, e, mod);
        result = (result * pp) % (unsigned long long)mod;
    }

    return (seq_t)result;     /* cast back to seq_t */
}
