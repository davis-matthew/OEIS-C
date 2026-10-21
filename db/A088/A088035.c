/*
 * A088035  Cubes k such that k-1 is divisible by a cube >1.
 * https://oeis.org/A088035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A088036(n)]^3 where A088036(n) is the nth integer x such
 *            that x^3-1 is divisible by a cube >1
 * Range:     u8 none, u16 n <= 9, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  Prime table (up to 200 000)  ---------- */
#define PRIME_MAX 200000
static uint32_t primes[PRIME_MAX];
static size_t prime_count = 0;

/* Generate the prime lookup table once. */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    static unsigned char sieve[PRIME_MAX + 1];
    for (int i = 0; i <= PRIME_MAX; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (int i = 2; (long long)i * i <= PRIME_MAX; ++i)
        if (sieve[i])
            for (int j = i * i; j <= PRIME_MAX; j += i)
                sieve[j] = 0;
    for (int i = 2; i <= PRIME_MAX; ++i)
        if (sieve[i]) primes[prime_count++] = (uint32_t)i;
    done = true;
}

/* ----------  Caching of a(n)  ---------- */
static seq_t *cache = NULL;
static size_t cache_size     = 0;
static size_t cache_capacity = 0;
static uint64_t last_x   = 1;
static bool initialized = false;

static void ensure_cache_capacity(size_t need)
{
    if (cache_capacity >= need) return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);               /* out of memory – abort */
    cache = tmp;
    cache_capacity = new_cap;
}

/* ----------  Check whether x^3-1 has a cube divisor >1  ---------- */
static bool has_cube_divisor(uint64_t x)
{
    seq_t t = (seq_t)x;
    t *= x;
    t *= x;
    t -= 1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if (p > x) break;
        seq_t p3 = (seq_t)p;
        p3 *= p;
        p3 *= p;
        if ((t % p3) == 0) return true;
    }
    return false;
}

/* ----------  Main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1; values <1 undefined */
    size_t N = (size_t)n;
    if (!initialized) { initialized = true; init_primes(); }

    /* Grow cache until we have at least N terms. */
    if (cache_size < N) {
        ensure_cache_capacity(N);
        while (cache_size < N) {
            ++last_x;
            if (has_cube_divisor(last_x)) {
                seq_t val = (seq_t)last_x;
                val *= last_x;
                val *= last_x;
                cache[cache_size++] = val;
            }
        }
    }
    return cache[N - 1];
}
