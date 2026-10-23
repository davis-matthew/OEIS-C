/*
 * A042668  Numerators of continued fraction convergents to sqrt(864).
 * https://oeis.org/A042668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042668: Numerators of continued fraction convergents to sqrt(864).
   Recurrence for n ≥ 24:
        a(n) = 940898 · a(n-12) – a(n-24)
   Initial values a(0)…a(23) are the numerators of the convergents
   obtained by the standard continued‑fraction expansion of √864.  The
   function computes those initial values algorithmically, then
   extends the cache with the linear recurrence.  The implementation
   works with any seq_t/idx_t pair defined by the build. */

#include <stdlib.h>

/* Integer floor‑sqrt for unsigned integer type idx_t.  Only needed
   for the small constant D = 864. */
static idx_t int_sqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Compute a(n).  Uses lazy, memoised cache so calls may be out of
   order. */
seq_t a(idx_t n)
{
    /* --- static state --------------------------------------------------- */
    static seq_t *cache   = NULL;  /* array of computed values         */
    static idx_t  cap     = 0;     /* current capacity                  */
    static idx_t  len     = 0;     /* number of stored elements         */
    static int    init    = 0;     /* whether the cache has been
                                      initialised with first 24 terms     */

    /* --- initialise cache with the first 24 numerators ----------------- */
    if (!init)
    {
        /* Allocate an initial capacity and store the first 24 elements. */
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* allocation failure – unlikely */

        /* Continued fraction data for sqrt(864). */
        const idx_t D      = 864;
        const idx_t sqrtD  = int_sqrt_idx(D);

        idx_t  m = 0, d = 1, a_val = sqrtD;
        seq_t p_m2 = 1;          /* p_{-1} */
        seq_t p_m1 = a_val;      /* p_0   */
        cache[0] = p_m1;
        len     = 1;             /* one element stored */

        for (idx_t i = 1; i < 24; ++i)
        {
            idx_t m_next = d * a_val - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (sqrtD + m_next) / d_next;

            seq_t p_next = a_next * p_m1 + p_m2;
            cache[i] = p_next;

            /* Shift for next iteration. */
            p_m2 = p_m1;
            p_m1 = p_next;
            m    = m_next;
            d    = d_next;
            a_val = a_next;
        }
        len  = 24;
        init = 1;
    }

    /* --- extend cache up to requested index n -------------------------- */
    while (len <= n)
    {
        /* Grow the array if necessary. */
        if (len >= cap)
        {
            size_t new_cap = cap ? (cap << 1) : 64;
            while (new_cap <= len)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;  /* allocation failure – unlikely */
            cache = tmp;
            cap   = new_cap;
        }

        /* Linear recurrence: a(n) = 940898 * a(n-12) – a(n-24). */
        seq_t val = (seq_t)940898 * cache[len - 12] - cache[len - 24];
        cache[len] = val;
        ++len;
    }

    return cache[(size_t)n];
}
