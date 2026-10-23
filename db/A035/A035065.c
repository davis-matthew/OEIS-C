/*
 * A035065  Numbers k such that k! has a prime number of digits.
 * https://oeis.org/A035065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k such that k! has a prime number of decimal digits
 * Range:     u8 n <= 53, u16 n <= 5769, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* small primality test for the number of digits (int <= ~40000) */
static bool is_prime_int(int x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0 || x % 3 == 0) return false;
    for (int i = 5; (long long)i * i <= x; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }
    return true;
}

/* cache for the sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* state for incremental factorial digit count */
static idx_t current_k = 1;          /* factorial of current_k held implicitly */
static long double log_sum = 0.0L;   /* sum of log10(i) for i=1..current_k */

/* ensure cache has space for at least 'n' terms */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap <<= 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* generate sequence values up to index 'n' (1‑based) */
static void generate_up_to(idx_t n)
{
    if (cache_len >= n) return;

    while (cache_len < n) {
        ++current_k;                     /* next k */
        log_sum += log10l((long double)current_k);
        /* digits = floor(log10(k!)) + 1 */
        int digits = (int)(floorl(log_sum + 1e-12L)) + 1;
        if (is_prime_int(digits)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)current_k;
        }
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but avoid UB */
    generate_up_to(n);
    return cache[n - 1];
}
