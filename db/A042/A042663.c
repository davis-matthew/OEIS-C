/*
 * A042663  Denominators of continued fraction convergents to sqrt(861).
 * https://oeis.org/A042663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042663: Denominators of continued‑fraction convergents to sqrt(861)
   Recurrence used for n >= 32:
   a(n) = 1083203602 * a(n-16) - a(n-32)
   Initial values a(0) … a(31) are generated from the continued fraction of sqrt(861). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t init[32];
static bool init_done = false;

static void init_first32(void)
{
    if (init_done) return;
    init_done = true;

    const uint64_t D = 861;
    const uint64_t a0 = 29;          /* floor(sqrt(861)) */

    uint64_t m = 0, d = 1, a = a0;
    seq_t q_prevprev = 1;           /* q_{-2} */
    seq_t q_prev     = 0;           /* q_{-1} */

    for (idx_t i = 0; i < 32; ++i) {
        /* q_i = a_i * q_{i-1} + q_{i-2} */
        seq_t q_cur = (seq_t)a * q_prev + q_prevprev;
        init[i] = q_cur;

        /* update continued‑fraction parameters */
        uint64_t m_next = d * a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;

        q_prevprev = q_prev;
        q_prev     = q_cur;
    }
}

static seq_t *cache = NULL;
static size_t  cache_cap = 0;
static idx_t  cache_len = 0;

static void ensure_capacity(idx_t n)
{
    if ((idx_t)cache_cap > n) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    init_first32();

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 32; ++i) cache[i] = init[i];
        cache_len = 32;
    }

    if (n < cache_len) return cache[n];

    ensure_capacity(n);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 32) {
            val = init[i];
        } else {
            val = ((seq_t)1083203602 * cache[i-16]) - cache[i-32];
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
