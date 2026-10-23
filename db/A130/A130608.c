/*
 * A130608  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+167)^2 = y^2.
 * https://oeis.org/A130608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A130608: nonnegative values x of solutions (x,y) to
 *   x^2 + (x+167)^2 = y^2.
 *
 * Recurrence:  a(1)=0, a(2)=28, a(3)=385, a(4)=501, a(5)=645, a(6)=2668;
 *              a(n)=6*a(n-3)-a(n-6)+334   for n>6.
 *
 * This implementation keeps a simple cache so calls in any order are fast.
 */

#include <stdlib.h>

/* 1‑based cache: a_cache[0] is unused. */
static seq_t *a_cache = NULL;   /* array of cached values */
static idx_t  a_last  = 0;      /* largest n for which a_cache[n] is valid */
static size_t a_cap   = 0;      /* allocated size (always > a_last) */

/* initialise cache with the first six terms */
static void a_init(void)
{
    a_cap = 7;                                 /* indices 0..6 */
    a_cache = (seq_t *)malloc(sizeof(seq_t) * a_cap);
    if (!a_cache) abort();                    /* out of memory – not expected */
    a_cache[0] = 0;                            /* unused */
    a_cache[1] = 0;    /* a(1) */
    a_cache[2] = 28;   /* a(2) */
    a_cache[3] = 385;  /* a(3) */
    a_cache[4] = 501;  /* a(4) */
    a_cache[5] = 645;  /* a(5) */
    a_cache[6] = 2668; /* a(6) */
    a_last = 6;
}

/* ensure that values up to n are cached */
static void a_ensure(idx_t n)
{
    if (a_cache == NULL) a_init();

    if (n <= a_last) return;   /* already cached */

    /* grow the allocation if necessary */
    size_t new_cap = a_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until large enough */
    if (new_cap > a_cap) {
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* out of memory – unexpected */
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t t = 6 * a_cache[i - 3];
        t -= a_cache[i - 6];
        t += 334;
        a_cache[i] = t;
    }
    a_last = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* sequence is 1‑based; 0 is unused */

    /* fast path for the first six terms */
    if (n < 7) {
        switch ((int)n) {
            case 1: return 0;
            case 2: return 28;
            case 3: return 385;
            case 4: return 501;
            case 5: return 645;
            case 6: return 2668;
            default: return 0;   /* should never happen */
        }
    }

    a_ensure(n);
    return a_cache[n];
}
