/*
 * A084679  Composite numbers with coprime numbers of prime factors with and
 *          without repetition.
 * https://oeis.org/A084679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9760 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth composite number m such that
 * ω(m) > 1 and gcd(Ω(m), ω(m)) = 1,
 * where ω(m) is the number of distinct prime factors and
 * Ω(m) is the total number of prime factors (with multiplicity). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Return true if x >= 2 is prime (deterministic trial division) */
static inline bool is_prime(idx_t x)
{
    if (x < 2)                         return false;
    if (x == 2 || x == 3)              return true;
    if (x % 2 == 0)                    return false;
    for (idx_t i = 3; i <= x / i; i += 2)
        if (x % i == 0)                return false;
    return true;
}

static seq_t bigomega(idx_t n)
{
    /* Count prime factors of n with multiplicity. */
    if (n <= 1)
        return 0;

    seq_t count = 0;

    /* Count factors of 2. */
    while ((n & 1) == 0) {
        n >>= 1;
        ++count;
    }

    /* Test odd divisors up to sqrt(n). */
    for (idx_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0) {
            n /= p;
            ++count;
        }
    }

    /* If n > 1 now, it is a prime factor > sqrt(original n). */
    if (n > 1)
        ++count;

    return count;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

/* Greatest common divisor for seq_t values. */
static seq_t gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* stored valid terms */
    static size_t cache_sz = 0;      /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t next_test = 4;      /* next integer to test */

    /* Ensure the cache array can hold at least n terms */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them */
    while (cache_sz < (size_t)n) {
        /* Skip primes and numbers with only one distinct prime factor */
        if (!is_prime(next_test) &&
            count_distinct_primes((uint64_t)next_test) > 1) {
            int omega = count_distinct_primes((uint64_t)next_test);
            seq_t total = bigomega((idx_t)next_test);
            if (gcd((seq_t)omega, total) == 1) {
                cache[cache_sz++] = (seq_t)next_test;
            }
        }
        ++next_test;
    }

    return cache[n - 1];
}
