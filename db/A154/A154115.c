/*
 * A154115  Numbers n such that n + 3 is prime.
 * https://oeis.org/A154115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n+1) - 3 (offset = 1)
 * Range:     u8 n <= 54, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154115: Numbers n such that n + 3 is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- integer square root for uint64_t ------------------------------------ */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n >> 1u;
    if (hi < 2u) hi = n;          /* for n=0,1,2,3 */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1u;
        if (mid == 0u) return 0u;
        if (mid > n / mid) {
            hi = mid - 1u;
        } else {
            if ((mid + 1u) > n / (mid + 1u))
                return mid;
            lo = mid + 1u;
        }
    }
    return hi;
}

/* --- prime sieve (trial division) --------------------------------------- */
static uint32_t *primes = NULL;          /* array of found primes */
static size_t primes_len = 0;            /* number of primes stored */
static size_t primes_cap = 0;            /* current capacity */

/* Ensure at least 'needed' primes are generated (indexing starts at 1). */
static void ensure_primes(size_t needed)
{
    if (primes_len >= needed)
        return;

    /* Grow storage if necessary */
    if (primes_len == 0) {
        /* allocate initial capacity */
        primes_cap = 16;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) exit(1);
        primes[0] = 2u;                 /* first prime: 2 */
        primes_len = 1;
    } else if (primes_cap < needed) {
        size_t new_cap = primes_cap;
        while (new_cap < needed) new_cap <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp) exit(1);
        primes = tmp;
        primes_cap = new_cap;
    }

    /* Find further primes */
    uint32_t candidate = primes[primes_len - 1] + 1u;
    while (primes_len < needed) {
        /* skip even numbers > 2 */
        if ((candidate & 1u) == 0u) { candidate++; continue; }

        bool is_prime = true;
        idx_t lim = isqrt_u64(candidate);
        for (size_t i = 0; i < primes_len; ++i) {
            uint32_t p = primes[i];
            if ((idx_t)p > lim) break;
            if (candidate % p == 0u) { is_prime = false; break; }
        }
        if (is_prime) {
            primes[primes_len++] = candidate;
        }
        candidate++;
    }
}

/* --- the required sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    /* sequence offset is 1: a(1) corresponds to n=1.  For n=0 behavior is unused. */
    if (n == 0u) return (seq_t)0;          /* irrelevant, but defined */

    /* We need the (n+1)-th prime, because prime_1 = 2 gives n = -1 which is not used. */
    size_t target = (size_t)n + 1u;        /* 1‑based index */
    ensure_primes(target);

    seq_t p = (seq_t)primes[target - 1u];
    return p - (seq_t)3u;
}
