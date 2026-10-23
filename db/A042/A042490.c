/*
 * A042490  Numerators of continued fraction convergents to sqrt(773).
 * https://oeis.org/A042490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent of √773. Recurrence: Let
 *            p_{-1} = 1, p_0 = ⌊√773⌋ = 27. For k ≥ 0 let m_0 = 0, d_0 = 1,
 *            a0 = p_0. m_{k+1} = d_k * a_k − m_k d_{k+1} = (773 – m_{k+1}²) /
 *            d_k a_{k+1} = (a0 + m_{k+1}) / d_{k+1} p_{k+1} = a_{k+1} * p_k +
 *            p_{k-1} Return p_n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 13, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#if defined(__cplusplus)
#include <vector>
#endif

/* integer square‑root for 64‑bit values (needed only for initial a0). */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t r = 0;
    for (uint64_t bit = 1ULL << 62; bit; bit >>= 1) {
        uint64_t next = r | bit;
        if ((unsigned __int128)next * next <= n)
            r = next;
    }
    return r;
}

#if defined(__cplusplus)
/* ----------------- C++ build (arbitrary‑precision) ----------------- */
static std::vector<seq_t> cache;
static uint64_t a0, m, d, a_cur;
static seq_t p_prevprev, p_prev;
static bool initialized = false;

static inline void init_state(void)
{
    a0     = isqrt_uint64(773);
    a_cur  = a0;
    m      = 0;
    d      = 1;
    p_prevprev = (seq_t)1;
    p_prev     = (seq_t)a0;
    cache.clear();
    cache.push_back(p_prev);   /* index 0 */
    initialized = true;
}
#else
/* ----------------- C build (fixed‑size integers) ------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t a0, m, d, a_cur;
static seq_t p_prevprev, p_prev;
static bool initialized = false;

static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)
        abort();
    cache = newp;
    cache_cap = new_cap;
}

static void init_state(void)
{
    a0     = isqrt_uint64(773);
    a_cur  = a0;
    m      = 0;
    d      = 1;
    p_prevprev = (seq_t)1;
    p_prev     = (seq_t)a0;
    ensure_cache_cap(1);
    cache[0] = p_prev;   /* index 0 */
    cache_len = 1;
    initialized = true;
}
#endif

seq_t a(idx_t n)
{
    if (!initialized)
        init_state();

#if defined(__cplusplus)
    while (cache.size() <= (size_t)n) {
        /* compute next coefficient a_{k+1} */
        uint64_t m_next = d * a_cur - m;
        uint64_t d_next = (773 - (uint64_t)((unsigned __int128)m_next * m_next)) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        seq_t p_next = (seq_t)a_next * p_prev + p_prevprev;
        cache.push_back(p_next);

        /* advance continued‑fraction state */
        m = m_next;
        d = d_next;
        a_cur = a_next;
        p_prevprev = p_prev;
        p_prev = p_next;
    }
    return cache[(size_t)n];
#else
    if (cache_len <= (size_t)n) {
        size_t target = (size_t)n + 1;
        ensure_cache_cap(target);
        for (size_t i = cache_len; i < target; ++i) {
            if (i == 0) /* already stored */ {
                continue;
            }
            /* next coefficient */
            uint64_t m_next = d * a_cur - m;
            uint64_t d_next = (773 - (uint64_t)((unsigned __int128)m_next * m_next)) / d;
            uint64_t a_next = (a0 + m_next) / d_next;

            seq_t p_next = (seq_t)a_next * p_prev + p_prevprev;
            cache[i] = p_next;

            /* advance state */
            m = m_next;
            d = d_next;
            a_cur = a_next;
            p_prevprev = p_prev;
            p_prev = p_next;
        }
        cache_len = target;
    }
    return cache[(size_t)n];
#endif
}
