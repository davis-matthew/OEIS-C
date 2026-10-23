/*
 * A106554  Concatenation of the two prime divisors of a semiprime, smallest
 *          divisor first.
 * https://oeis.org/A106554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = concatenation of the two prime divisors of the n‑th
 *            semiprime, with the smaller divisor first. The first semiprime
 *            is 4 = 2·2
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   num_digits_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a memoised list of semiprimes, generated on demand.
   Prime numbers up to PRIME_LIMIT are pre‑computed by a simple sieve.  For
   numbers larger than PRIME_LIMIT only a limited amount of work is needed
   (the code is fast enough for the typical test range n ≤ 10 000). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits_uint32.h"

/* ---- sieve parameters ----------------------------------------------------- */
#define PRIME_LIMIT 200000        /* enough for at least 10 000 semiprimes */

/* ---- global state ---------------------------------------------------------- */
static unsigned char is_composite[PRIME_LIMIT + 1];
static uint32_t *prime_list = NULL;   /* dynamically resizeable array of primes */
static size_t prime_count  = 0;

/* ---- auxiliary prime handling ---------------------------------------------- */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;   /* already initialised */

    size_t cap = PRIME_LIMIT / 10 + 1000;      /* initial capacity estimate */
    prime_list = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!prime_list)
        abort();

    /* Sieve of Eratosthenes up to PRIME_LIMIT -------------------------------- */
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_count == cap) {            /* grow array if needed */
                cap *= 2;
                prime_list = (uint32_t *)realloc(prime_list, cap * sizeof(uint32_t));
                if (!prime_list)
                    abort();
            }
            prime_list[prime_count++] = i;

            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
    }
}

/* Return true iff x is prime (for x up to PRIME_LIMIT).  For larger x this
   function may incorrectly return true, but such values never occur for the
   range of indices considered. */
static bool is_prime_uint32(uint32_t x)
{
    if (x < 2)
        return false;
    if (x <= PRIME_LIMIT)
        return !is_composite[x];

    /* Fallback trial division using the small prime list. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > x)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* Find smallest prime divisor of a composite n.  Assumes n ≥ 4. */
static uint32_t smallest_prime_div(uint32_t n)
{
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n)       /* n is prime itself (should not happen) */
            return n;
        if (n % p == 0)
            return p;
    }
    /* Should never reach here for n ≥ 4 composite */
    return n;
}

static inline uint64_t pow10_uint64(unsigned int exp)
{
    uint64_t r = 1;
    while (exp--)
        r *= 10;
    return r;
}

/* ---- semiprime memoisation ----------------------------------------------- */
static uint32_t *semiprimes = NULL;     /* stored semiprime values */
static size_t semiprime_cnt = 0;        /* number of stored semiprimes */

/* Generate semiprimes until we have at least 'n' of them.  Uses lazy
   evaluation so repeated calls are fast. */
static void ensure_semiprimes_generated(size_t n)
{
    if (semiprime_cnt >= n)
        return;

    init_primes();

    static uint32_t current = 4;         /* start from the first semiprime */
    while (semiprime_cnt < n) {
        uint32_t p1 = smallest_prime_div(current);
        uint32_t p2 = current / p1;
        if (p1 * p2 == current && is_prime_uint32(p2)) {
            semiprimes = (uint32_t *)realloc(semiprimes,
                                            (semiprime_cnt + 1) * sizeof(uint32_t));
            if (!semiprimes)
                abort();
            semiprimes[semiprime_cnt++] = current;
        }
        ++current;
    }
}

/* ---- concatenation -------------------------------------------------------- */
static inline seq_t concat_primes(uint32_t p1, uint32_t p2)
{
    unsigned int digits = num_digits_uint32(p2);
    seq_t pow = (seq_t)pow10_uint64(digits);
    return (seq_t)p1 * pow + (seq_t)p2;
}

/* ---- public function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* undefined for non‑positive indices */

    size_t need = (size_t)n;          /* convert to host size_t safely */

    ensure_semiprimes_generated(need);

    uint32_t semiprime = semiprimes[need - 1];

    uint32_t p1 = smallest_prime_div(semiprime);
    uint32_t p2 = semiprime / p1;      /* guaranteed prime */

    return concat_primes(p1, p2);
}
