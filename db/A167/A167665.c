/*
 * A167665  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            10*a(n-1)+10*a(n-2)+…+10*a(n-14)-55*a(n-15) for n ≥ 16 with
 *            a(0)=1, a(1)=12, …, a(14)=414272545727172 and
 *            a(15)=10*a(14)+…+10*a(1)-55*a(0)+1 (the extra +1 comes from the
 *            numerator of the generating function)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: recurrence comes from G(t) = (t^15+2t^14+…+1)/(55t^15-10t^14-…-10t+1) */

#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* -------------------- static cache ------------------------ */
    static seq_t *cache   = NULL;     /* contains computed terms 0..cache_sz-1 */
    static size_t cache_cap = 0;      /* allocated capacity in elements   */
    static size_t cache_sz  = 0;      /* number of known terms            */
    static bool initialized   = false;

    /* -------------------- initial 15 values ------------------- */
    static const seq_t base_vals[15] = {
        1ULL,          /* a[0] */
        12ULL,         /* a[1] */
        132ULL,        /* a[2] */
        1452ULL,       /* a[3] */
        15972ULL,      /* a[4] */
        175692ULL,     /* a[5] */
        1932612ULL,    /* a[6] */
        21258732ULL,   /* a[7] */
        233846052ULL,  /* a[8] */
        2572306572ULL, /* a[9] */
        28295372292ULL,/* a[10] */
        311249095212ULL,/* a[11] */
        3423740047332ULL,/* a[12] */
        37661140520652ULL,/* a[13] */
        414272545727172ULL/* a[14] */
    };

    /* ---------------- first use: allocate cache ---------------- */
    if (!initialized) {
        cache_cap = 15;
        cache = seq_alloc(cache_cap);          /* allocates cache_cap elements */
        for (size_t i = 0; i < 15; ++i)
            cache[i] = base_vals[i];
        cache_sz = 15;          /* terms 0 … 14 are now known */
        initialized = true;
    }

    /* --------------------- return known term ------------------- */
    if (n < cache_sz)
        return cache[(size_t)n];

    /* --------------------- grow cache if needed ---------------- */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n)
            new_cap <<= 1;                /* double until large enough */
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* --------------------- compute missing terms -------------- */
    for (idx_t k = (idx_t)cache_sz; k <= n; ++k) {
        seq_t sum = 0;
        /* terms a(k-1) … a(k-14) */
        for (int i = 1; i <= 14; ++i)
            sum += 10 * cache[(size_t)(k - i)];
        /* minus 55*a(k-15) */
        sum -= 55 * cache[(size_t)(k - 15)];
        /* if k == 15 add the +1 from the numerator of the G.F. */
        if (k == 15)
            sum += 1;
        cache[(size_t)k] = sum;
    }

    cache_sz = (size_t)(n + 1);   /* we now have terms 0 … n */
    return cache[(size_t)n];
}
