/*
 * A151743  Composite which are the sum of five consecutive composite numbers.
 * https://oeis.org/A151743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of five consecutive composite numbers in the list of
 *            composite numbers, taking only composite sums. Uses a sliding
 *            window over the composites
 * Range:     u8 n <= 26, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Small trial‑division primality test for 64‑bit values. */
static bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* OEIS A151743 */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* static state for generating composites */
    static uint64_t next_candidate = 4;      /* next integer to test */
    static uint64_t window[5] = {0};         /* last 5 composites */
    static size_t win_used = 0;              /* number of values in window */
    static size_t win_idx = 0;               /* index of oldest in window */
    static uint64_t sum = 0;                 /* sum of current window */

    /* if cache uninitialized, allocate small initial array */
    if (!cache) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* find next composite number */
        uint64_t c = next_candidate++;
        while (is_prime_small(c)) {            /* skip primes */
            c = next_candidate++;
        }

        if (win_used < 5) {                    /* fill initial window */
            sum += c;
            window[win_used++] = c;
            if (win_used == 5) {               /* window now full */
                if (!is_prime_small(sum)) {    /* composite sum */
                    if (cache_len >= cache_cap) {
                        cache_cap *= 2;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)sum;
                }
            }
        } else {                               /* slide window */
            sum -= window[win_idx];
            sum += c;
            window[win_idx] = c;
            win_idx = (win_idx + 1) % 5;
            if (!is_prime_small(sum)) {
                if (cache_len >= cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)sum;
            }
        }
    }

    return cache[n - 1];
}
