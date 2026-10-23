/*
 * A040816  Continued fraction for sqrt(846).
 * https://oeis.org/A040816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th coefficient of the simple continued fraction of sqrt(846) */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper: integer square root (floor of sqrt(n)) for small n */
static uint64_t int_sqrt_uint64(uint64_t n)
{
    uint64_t x = 0;
    while ((x + 1) * (x + 1) <= n) {
        ++x;
    }
    return x;
}

/* Cached data for generating terms */
static seq_t *cf_cache = NULL;           /* array of computed terms */
static size_t cf_cache_len = 0;          /* number of terms stored */
static size_t cf_cache_cap = 0;          /* allocated capacity of cf_cache */
static uint64_t cf_state_m = 0;          /* current m in continued‑fraction algorithm */
static uint64_t cf_state_d = 1;          /* current d in algorithm */
static uint64_t cf_state_a = 0;          /* current a in algorithm */
static uint64_t cf_a0 = 0;               /* floor(sqrt(846)) */

/* Ensure the cache has capacity for at least 'need' elements */
static void cf_ensure_capacity(size_t need)
{
    if (cf_cache_cap >= need)
        return;
    size_t new_cap = cf_cache_cap == 0 ? 8 : cf_cache_cap * 2;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a real implementation we would handle memory errors.
           For this problem we assume allocation succeeds. */
        return;
    }
    if (cf_cache) {
        memcpy(new_cache, cf_cache, cf_cache_len * sizeof(seq_t));
        free(cf_cache);
    }
    cf_cache = new_cache;
    cf_cache_cap = new_cap;
}

/* Initialize the continued‑fraction state on first use */
static void cf_initialize(void)
{
    if (cf_cache_len > 0)
        return;            /* already initialized */

    const uint64_t D = 846ULL;
    cf_a0 = int_sqrt_uint64(D);          /* floor(sqrt(846)) */
    cf_state_m = 0;
    cf_state_d = 1;
    cf_state_a = cf_a0;

    cf_ensure_capacity(1);
    cf_cache[0] = (seq_t)cf_a0;           /* a(0) */
    cf_cache_len = 1;
}

/* Generate terms up to index n */
static void cf_generate_to(idx_t n)
{
    cf_initialize();
    size_t target = (size_t)n;
    if (target < cf_cache_len)
        return;            /* already have this term */

    cf_ensure_capacity(target + 1);
    const uint64_t D = 846ULL;
    for (size_t i = cf_cache_len; i <= target; ++i) {
        /* Compute the next term */
        uint64_t m_next = cf_state_d * cf_state_a - cf_state_m;
        uint64_t d_next = (D - m_next * m_next) / cf_state_d;
        uint64_t a_next = (cf_a0 + m_next) / d_next;

        cf_cache[i] = (seq_t)a_next;   /* store */
        /* Update state for next iteration */
        cf_state_m = m_next;
        cf_state_d = d_next;
        cf_state_a = a_next;
    }
    cf_cache_len = target + 1;
}

seq_t a(idx_t n)
{
    cf_generate_to(n);
    return cf_cache[(size_t)n];
}
