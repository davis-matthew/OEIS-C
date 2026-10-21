/*
 * A200205  Number of -n..n arrays x(0..3) of 4 elements with zero sum and
 *          nonzero second and third differences.
 * https://oeis.org/A200205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) * + 2*a(n-5) -
 *            a(n-6) + 2*a(n-7) - a(n-8) for n >= 9 * Initial values: a(1)=12,
 *            a(2)=68, a(3)=186, a(4)=422, * a(5)=798, a(6)=1316, a(7)=2064,
 *            a(8)=3048
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* 
 * Computes the nth term (1‑based) of OEIS sequence A200205.
 * The function works for any seq_t / idx_t combination supplied
 * by the build system.  It uses a simple linear recurrence and
 * memoises previously computed values in a dynamically sized
 * array.  All arithmetic is performed in the seq_t type, so
 * truncation modulo the type width is automatic for small
 * integer builds (e.g. uint8_t).
 */
seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first stored value. */
    if (n == 0) return (seq_t)0;

    /* Static initializer: computed once per program execution. */
    static seq_t init_vals[8];
    static bool init_done = false;
    if (!init_done) {
        /* Initialise with full 32‑bit constants for larger types.
           For 8‑bit types we pre‑modulo the values to avoid
           compile‑time overflow warnings. */
        if (sizeof(seq_t) <= 1) {           /* 8‑bit */
            init_vals[0] = (seq_t)12;
            init_vals[1] = (seq_t)68;
            init_vals[2] = (seq_t)186;
            init_vals[3] = (seq_t)166;     /* 422 modulo 256 */
            init_vals[4] = (seq_t)30;      /* 798 modulo 256 */
            init_vals[5] = (seq_t)36;      /* 1316 modulo 256 */
            init_vals[6] = (seq_t)16;      /* 2064 modulo 256 */
            init_vals[7] = (seq_t)232;     /* 3048 modulo 256 */
        } else {
            init_vals[0] = (seq_t)12;
            init_vals[1] = (seq_t)68;
            init_vals[2] = (seq_t)186;
            init_vals[3] = (seq_t)422;
            init_vals[4] = (seq_t)798;
            init_vals[5] = (seq_t)1316;
            init_vals[6] = (seq_t)2064;
            init_vals[7] = (seq_t)3048;
        }
        init_done = true;
    }

    /* Return initial values directly if requested. */
    if (n <= 8) return init_vals[n - 1];

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;     /* index 1.. */
    static idx_t  stored = 0;       /* largest n already computed */
    static size_t capacity = 0;     /* allocated length of cache array */

    /* First call: allocate space and fill initial terms. */
    if (cache == NULL) {
        /* Ensure at least space for indices 1..max(n,8). */
        capacity = (size_t)(n > 8 ? n : 8) + 1;   /* +1 to make indices match n */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) { /* out of memory – return 0 as safe fallback */
            return (seq_t)0;
        }
        /* Store initial terms at indices 1..8. */
        for (idx_t i = 1; i <= 8; ++i) cache[i] = init_vals[i - 1];
        stored = 8;
    }

    /* Grow cache if necessary. */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – return cached value if possible */
            return cache[stored];
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = stored + 1; i <= n; ++i) {
        seq_t pos = 2 * cache[i - 1] + 2 * cache[i - 3] +
                    2 * cache[i - 5] + 2 * cache[i - 7];
        seq_t neg = cache[i - 2] + 4 * cache[i - 4] +
                    cache[i - 6] + cache[i - 8];
        cache[i] = pos - neg;
    }
    stored = n;

    return cache[n];
}
