/*
 * A106550  a(n) = n-th semiprime + (concatenation of its two prime factors,
 *          smallest factor first).
 * https://oeis.org/A106550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = semiprime(n) + concatenation of its two prime factors The
 *            sequence uses the n‑th semiprime (1‑based). This implementation
 *            sieve primes up to a fixed limit and builds the semiprime list
 *            once. Consequent calls reuse the pre‑computed data
 * Range:     u8 n <= 10, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Nominal sieve limit – sufficient for n up to about 10 000 */
static const uint64_t LIMIT = 200000U;

static bool            initialized   = false;
static uint64_t        *semiprimes   = NULL;
static size_t          semicap       = 0;
static size_t          semicount     = 0;
static uint64_t        *prime_list   = NULL;
static size_t          primecap      = 0;
static size_t          primecount    = 0;

/* Count decimal digits of a non‑negative integer. */
static size_t digits_uint64(uint64_t x)
{
    size_t d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* Helper: generate primes up to LIMIT and collect semiprimes. */
static void init_sequence(void)
{
    if (initialized) return;

    /* Sieve of Eratosthenes */
    uint8_t *is_prime = (uint8_t*)malloc((LIMIT + 1) * sizeof(uint8_t));
    for (uint64_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;

    /* Build list of primes */
    primecap   = 20000;
    prime_list = (uint64_t*)malloc(primecap * sizeof(uint64_t));
    primecount = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) {
            if (primecount == primecap) {
                primecap <<= 1;
                prime_list = (uint64_t*)realloc(prime_list, primecap * sizeof(uint64_t));
            }
            prime_list[primecount++] = i;
        }

    /* Build list of semiprimes */
    semicap    = 40000;
    semiprimes = (uint64_t*)malloc(semicap * sizeof(uint64_t));
    semicount  = 0;
    for (uint64_t num = 4; num <= LIMIT; ++num) {
        uint64_t p = 0;
        for (size_t k = 0; k < primecount && (uint64_t)prime_list[k] * prime_list[k] <= num; ++k) {
            if (num % prime_list[k] == 0) {
                p = prime_list[k];
                break;
            }
        }
        if (p == 0) continue;                 /* not a semiprime candidate */
        uint64_t q = num / p;
        if (q <= LIMIT && is_prime[q]) {
            if (semicount == semicap) {
                semicap <<= 1;
                semiprimes = (uint64_t*)realloc(semiprimes, semicap * sizeof(uint64_t));
            }
            semiprimes[semicount++] = num;
        }
    }

    free(is_prime);
    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_sequence();

    if (n == 0) return (seq_t)0;                     /* undefined for offset 1 */
    if (n > (idx_t)semicount) return (seq_t)0;        /* safety check */

    /* n is 1‑based; fetch the n‑th semiprime */
    uint64_t semino = semiprimes[(size_t)n - 1];

    /* Smallest prime factor – use the pre‑computed prime list. */
    uint64_t p = 0;
    for (size_t i = 0; i < primecount && (uint64_t)prime_list[i] * prime_list[i] <= semino; ++i) {
        if (semino % prime_list[i] == 0) {
            p = prime_list[i];
            break;
        }
    }
    if (p == 0) return (seq_t)semino;                  /* should not happen */

    uint64_t q = semino / p;                           /* other factor (q ≥ p) */

    /* Concatenation of p and q: p * 10^digits(q) + q. */
    size_t d     = digits_uint64(q);
    uint64_t pow10 = 1;
    for (size_t i = 0; i < d; ++i) pow10 *= 10ULL;
    uint64_t concat_val = (uint64_t)p * pow10 + q;

    /* Result: semino + concatenated value, modulo seq_t width. */
    uint64_t result = semino + concat_val;
    return (seq_t)result;
}
