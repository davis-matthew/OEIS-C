/*
 * A121725  Generalized central coefficients for k=3.
 * https://oeis.org/A121725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+1)*a(n) = 10*(n+1)*a(n-1) + 36*(n-2)*a(n-2) - 360*(n-2)*a(n-3)  (n≥3)
   Initial values: a(0)=1, a(1)=1, a(2)=10. */

/* required headers */
#include <stdlib.h>

/* ---------- Implementation using 128‑bit arithmetic for the C build ---------- */
/*  The code that uses __int128 is compiled only when compiling in C
    and when the compiler supports 128‑bit integers.  For C++ or when
    __int128 is not available (e.g. big‑integer builds) the generic
    implementation below is used.  It relies on the seq_t multiplication
    operators provided by the build, which never overflow for arbitrary‑precision
    types.  For a uint64_t build compiled in C, the 128‑bit version avoids
    intermediate overflow. */
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)

/* 64‑bit unsigned integer build (C) using __int128 to avoid overflow */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* allocated sequence values            */
    static size_t  cap    = 0;       /* current allocated size              */
    static size_t  count  = 0;       /* number of values stored             */

    /* initialise cache for the first three terms when needed */
    if (count < 3) {
        size_t new_cap = 3;
        if (cap < new_cap) {
            void *tmp = realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            cache = (seq_t *)tmp;
            cap   = new_cap;
        }
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 10;
        count = 3;
    }

    /* if the requested value is already computed, return it */
    if (n < count) return cache[n];

    /* grow cache capacity if necessary */
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap <= n) new_cap <<= 1;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* allocation failure */
        cache = (seq_t *)tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = count; i <= n; ++i) {
        if (i == 0) { cache[i] = 1; continue; }      /* initial values */
        if (i == 1) { cache[i] = 1; continue; }
        if (i == 2) { cache[i] = 10; continue; }

        unsigned __int128 term1 = (unsigned __int128)cache[i-1] * 10 * (i + 1);
        unsigned __int128 term2 = (unsigned __int128)cache[i-2] * 36 * (i - 2);
        unsigned __int128 term3 = (unsigned __int128)cache[i-3] * 360 * (i - 2);
        unsigned __int128 rhs   = term1 + term2 - term3;
        cache[i] = (seq_t)(rhs / (i + 1));
    }

    count = n + 1;            /* updated number of stored values */
    return cache[n];
}

#else   /* ---------- Generic implementation (C++ or when __int128 unavailable) ---------- */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* allocated sequence values            */
    static size_t  cap    = 0;       /* current allocated size              */
    static size_t  count  = 0;       /* number of values stored             */

    /* initialise cache for the first three terms when needed */
    if (count < 3) {
        size_t new_cap = 3;
        if (cap < new_cap) {
            void *tmp = realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            cache = (seq_t *)tmp;
            cap   = new_cap;
        }
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 10;
        count = 3;
    }

    /* if the requested value is already computed, return it */
    if (n < count) return cache[n];

    /* grow cache capacity if necessary */
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap <= n) new_cap <<= 1;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* allocation failure */
        cache = (seq_t *)tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = count; i <= n; ++i) {
        if (i == 0) { cache[i] = 1; continue; }
        if (i == 1) { cache[i] = 1; continue; }
        if (i == 2) { cache[i] = 10; continue; }

        /* Use the recurrence:
           (i+1)*a[i] = 10*(i+1)*a[i-1] + 36*(i-2)*a[i-2] - 360*(i-2)*a[i-3] */
        seq_t term1 = 10 * (i + 1) * cache[i - 1];
        seq_t term2 = 36 * (i - 2) * cache[i - 2];
        seq_t term3 = 360 * (i - 2) * cache[i - 3];
        seq_t rhs   = term1 + term2 - term3;
        cache[i]    = rhs / (i + 1);
    }

    count = n + 1;            /* updated number of stored values */
    return cache[n];
}
#endif
