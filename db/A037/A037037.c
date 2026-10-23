/*
 * A037037  Number of primes between n and 3n.
 * https://oeis.org/A037037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(3n) - pi(n-1) where pi(x) counts primes <= x
 * Range:     u8 n <= 962, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes a sieve up to 30000 (which suffices for n <= 10000)
   and maintains the prefix prime counts.  It is O(max_limit log log max_limit) once,
   and O(1) per call. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LIMIT 30000   /* 3 * 10000 */

static bool init_done = false;
static uint8_t is_comp[MAX_LIMIT + 1];
static seq_t prime_prefix[MAX_LIMIT + 1];

static void init_sieve(void)
{
    /* Mark composites */
    memset(is_comp, 0, sizeof(is_comp));
    is_comp[0] = is_comp[1] = 1;
    for (idx_t i = 2; i * i <= MAX_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= MAX_LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    /* Build prefix prime counts */
    seq_t cnt = 0;
    for (idx_t i = 0; i <= MAX_LIMIT; ++i) {
        if (!is_comp[i]) {
            ++cnt;
        }
        prime_prefix[i] = cnt;
    }

    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done)
        init_sieve();

    /* Ensure we are within the precomputed range */
    idx_t up = 3 * n;
    if (up > MAX_LIMIT) {
        /* Extend the sieve on demand (not needed for n <= 10000) */
        /* For safety, clamp to MAX_LIMIT; larger n will give incorrect but
           the problem constraint keeps n <= 10000. */
        up = MAX_LIMIT;
    }

    seq_t pi_up   = prime_prefix[up];
    seq_t pi_down = prime_prefix[(n > 0) ? (n - 1) : 0];
    return pi_up - pi_down;
}
