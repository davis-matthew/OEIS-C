/*
 * A066328  a(n) = sum of indices of distinct prime factors of n; here,
 *          index(i-th prime) = i.
 * https://oeis.org/A066328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of indices of distinct prime factors of n (PrimePi(p)
 *            = index of prime p, 2 → 1, 3 → 2, 5 → 3, …)
 * Range:     u8 n <= 1618, u16 n <= 9872, u32 n <= 9872, u64 n <= 9872,
 *            u128 n <= 9872, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* ---- compile‑time configuration ---- */
#define PRIME_LIMIT_MAX 5000000          /* maximum number up to which primes are cached */
#define PRIME_ARRAY_SIZE 400000          /* enough for ~348 513 primes up to 5 000 000 */

/* ---- static storage ---- */
static uint8_t sieve[PRIME_LIMIT_MAX + 1];
static uint32_t primes[PRIME_ARRAY_SIZE];
static size_t prime_count = 0;
static int primes_initialized = 0;

/* ---- binary search for the 1‑based index of a prime ---- */
static int prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint32_t v = primes[mid];
        if (v == p) return (int)(mid + 1);   /* 1‑based index */
        if (v < p) lo = mid + 1;
        else       hi = mid;
    }
    return -1; /* should not happen for a prime divisor */
}

/* ---- one‑time sieve to fill the prime table up to PRIME_LIMIT_MAX ---- */
static void init_primes(void)
{
    if (primes_initialized) return;

    memset(sieve, 1, sizeof(sieve));
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= PRIME_LIMIT_MAX; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= PRIME_LIMIT_MAX; j += i)
                sieve[j] = 0;

    prime_count = 0;
    for (size_t i = 2; i <= PRIME_LIMIT_MAX; ++i)
        if (sieve[i])
            primes[prime_count++] = (uint32_t)i;

    primes_initialized = 1;
}

/* ---- required public function ---- */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;   /* a(1) = 0, a(0) is unused */

    init_primes();

    seq_t sum = 0;
    idx_t temp = n;

    /* trial division by primes up to sqrt(temp) */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)temp) break;
        if (temp % p == 0) {
            sum += (seq_t)(i + 1);   /* i+1 is the 1‑based prime index */
            while (temp % p == 0) temp /= p;
        }
    }

    /* if a prime factor > 1 remains, add its index */
    if (temp > 1) {
        int idx = prime_index((uint32_t)temp);
        if (idx > 0) sum += (seq_t)idx;
    }

    return sum;
}
