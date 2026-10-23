/*
 * A117285  Numbers k for which the cototient k-phi(k) is a pentagonal number.
 * https://oeis.org/A117285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 61, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A117285
    Numbers k for which the cototient k-phi(k) is a pentagonal number.
    The function computes the sequence by iterating over k,
    checking k-phi(k) against the pentagonal test,
    and caching the results for subsequent calls. */

/* ---------- standard headers ---------- */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- external utilities ---------- */
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) → seq_t */

/* ---------- helper: compute Euler phi for uint64_t ---------- */
static seq_t phi_uint64(uint64_t n)
{
    seq_t result = (seq_t)n;
    uint64_t temp  = n;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0) temp /= p;
            result -= result / (seq_t)p;
        }
    }
    if (temp > 1) result -= result / (seq_t)temp;
    return result;
}

/* ---------- helper: pentagonal test ---------- */
static bool is_pentagonal(seq_t x)
{
    if (x == (seq_t)0) return true;          /* P(0)=0 */
    seq_t val = 24 * x + (seq_t)1;
    seq_t r   = floor_sqrt(val);
    if (r * r != val) return false;
    return ((r + (seq_t)1) % (seq_t)6) == (seq_t)0;
}

/* ---------- static cache state ---------- */
static seq_t *cache      = NULL;   /* cached sequence values   */
static size_t cache_cap  = 0;      /* allocated capacity       */
static size_t cache_len  = 0;      /* number of stored values  */
static uint64_t next_k   = 1;      /* next integer to test     */

/* ---------- expand cache to contain at least 'need' values ---------- */
static void ensure_cache_length(size_t need)
{
    /* Grow cache array if required */
    if (cache_len < need && cache_cap < need)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache      = tmp;
        cache_cap  = new_cap;
    }

    while (cache_len < need)
    {
        /* Compute phi for current k */
        seq_t k_seq = (seq_t)next_k;
        seq_t phi_k = phi_uint64(next_k);
        seq_t cot   = k_seq - phi_k;

        if (is_pentagonal(cot))
        {
            cache[cache_len++] = k_seq;
        }
        ++next_k;
        /* Avoid infinite loops if uint64_t overflows */
        if (next_k == 0) break;
    }
}

/* ---------- public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined before offset */
    ensure_cache_length((size_t)n);
    return cache[n-1];
}
