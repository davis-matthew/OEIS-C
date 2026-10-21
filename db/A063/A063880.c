/*
 * A063880  Numbers k such that sigma(k) = 2*usigma(k).
 * https://oeis.org/A063880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 108 × b(n) where b(n) is the nth odd squarefree integer
 *            not divisible by 3. This implements the requirement that
 *            sigma(k) = 2·usigma(k)
 * Range:     u8 n <= 1, u16 n <= 184, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAXP 100000                 /* upper bound for sieve */

static bool sieve_initialized = false;
static uint32_t primes[10000];      /* enough for primes ≤ 100000 (9592 primes) */
static size_t prime_count = 0;
static bool composite[MAXP + 1];

/* Generate all primes up to MAXP only once */
static void init_sieve(void)
{
    if (sieve_initialized) return;
    for (size_t i = 0; i <= MAXP; ++i) composite[i] = false;
    composite[0] = composite[1] = true;
    for (size_t i = 2; i * i <= MAXP; ++i) {
        if (!composite[i]) {
            for (size_t j = i * i; j <= MAXP; j += i) composite[j] = true;
        }
    }
    size_t idx = 0;
    for (size_t i = 2; i <= MAXP && idx < sizeof(primes)/sizeof(primes[0]); ++i) {
        if (!composite[i]) primes[idx++] = (uint32_t)i;
    }
    prime_count = idx;
    sieve_initialized = true;
}

/* Test whether n is square‑free (no prime factor appears twice) */
static bool is_squarefree(uint64_t n)
{
    uint64_t x = n;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > x) break;
        if (x % p == 0) {
            if ((x / p) % p == 0) return false;  /* p² divides n */
            while (x % p == 0) x /= p;
        }
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; undefined for n<1 */

    init_sieve();                    /* make primes available */

    idx_t count = 0;
    idx_t candidate = 1;             /* start with 1 (odd, not divisible by 3) */

    while (count < n) {
        if (candidate % 3 != 0) {    /* odd integer not divisible by 3 */
            if (is_squarefree((uint64_t)candidate)) {
                ++count;
                if (count == n) {
                    seq_t val = (seq_t)(108ULL) * (seq_t)candidate;
                    return val;
                }
            }
        }
        candidate += 2;               /* next odd candidate */
    }
    return 0;                        /* unreachable */
}
