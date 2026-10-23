/*
 * A041521  Denominators of continued fraction convergents to sqrt(277).
 * https://oeis.org/A041521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=1, a(n)=a_partial(n)*a(n-1)+a(n-2) (n≥2) where
 *            a_partial(k) are the continued‐fraction partial quotients of
 *            sqrt(277)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
/* Cast helpers for C++ */
#define ALLOC(type, n) reinterpret_cast<type*>(std::malloc((n)*sizeof(type)))
#define REALLOC(ptr, type, n) reinterpret_cast<type*>(std::realloc((ptr), (n)*sizeof(type)))
#else
/* C version – cast is not needed */
#define ALLOC(type, n) ((type*)malloc((n)*sizeof(type)))
#define REALLOC(ptr, type, n) ((type*)realloc((ptr), (n)*sizeof(type)))
#endif

/* --- continued‑fraction data for sqrt(277) -------------------------------- */
static idx_t period_len = 0;      /* length of the periodic part (excluding a0) */
static idx_t *a_vals = NULL;      /* a_vals[0] = a0, a_vals[1..period_len] = period */

static void init_constants(void)
{
    if (period_len != 0)          /* already initialised */
        return;

    /* Constants for sqrt(277) */
    const idx_t D     = 277;
    const idx_t a0    = 16;        /* floor(sqrt(277)) */

    /* Temporary storage – grow as needed */
    size_t cap = 32, sz = 0;
    idx_t *tmp = ALLOC(idx_t, cap);
    if (!tmp) exit(1);

    /* a0 */
    tmp[sz++] = a0;

    /* First step to obtain a1 */
    idx_t m = 0;
    idx_t d = 1;
    m = d * a0 - m;
    d = (D - m * m) / d;
    idx_t an = (a0 + m) / d;

    /* Remember first state after a1 */
    const idx_t first_m = m;
    const idx_t first_d = d;

    /* push a1 */
    if (sz == cap) { cap <<= 1; tmp = REALLOC(tmp, idx_t, cap); }
    tmp[sz++] = an;

    /* Subsequent partial quotients until period repeats */
    for (;;)
    {
        m = d * an - m;
        d = (D - m * m) / d;
        an = (a0 + m) / d;

        if (m == first_m && d == first_d)
            break;   /* period closed */

        if (sz == cap) { cap <<= 1; tmp = REALLOC(tmp, idx_t, cap); }
        tmp[sz++] = an;
    }

    period_len = sz - 1;               /* exclude a0 */
    a_vals = REALLOC(tmp, idx_t, sz);
    if (!a_vals) exit(1);
}

/* Return the i‑th partial quotient (0‑based) of sqrt(277) */
static inline idx_t a_partial(idx_t i)
{
    if (i == 0)
        return a_vals[0];
    idx_t off = (i - 1) % period_len;   /* 1≤off≤period_len-1 */
    return a_vals[1 + off];
}

/* --- cache for the sequence values ----------------------------------- */
static seq_t *cache = NULL;   /* array of a(n) values, up to cache_len-1 */
static idx_t cache_len = 0;   /* number of elements stored in cache        */
static idx_t cache_cap = 0;   /* allocated capacity of cache                */

/* Ensure cache has space for at least `need` elements */
static void ensure_cache_cap(idx_t need)
{
    if (cache_cap >= need)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
    cache = REALLOC(cache, seq_t, new_cap);
    cache_cap = new_cap;
}

/* State variables for the recurrence (valid starting at i = cache_len) */
static seq_t prev2 = 0;    /* q_{i-2}  */
static seq_t prev1 = 1;    /* q_{i-1}  */

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_constants();

    /* First initialisation of cache on the first call */
    if (cache_len == 0)
    {
        ensure_cache_cap(1);
        cache[0] = (seq_t)1;    /* q0 = 1 */
        cache_len = 1;

        /* set the recurrence state for i = 1 */
        prev2 = 0;
        prev1 = 1;
    }

    if (n < cache_len)
        return cache[n];

    /* Compute missing terms up to n (inclusive) */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        idx_t an = a_partial(i);          /* partial quotient a_i */
        seq_t qi = an * prev1 + prev2;    /* q_i = a_i*q_{i-1} + q_{i-2} */

        ensure_cache_cap(cache_len + 1);
        cache[cache_len++] = qi;

        /* rotate state for next iteration */
        seq_t tmp = prev2;
        prev2 = prev1;
        prev1 = qi;
    }
    return cache[n];
}
