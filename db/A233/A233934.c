/*
 * A233934  Number of (n+1)X(1+1) 0..7 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 16.
 * https://oeis.org/A233934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula implemented below:
 * A233934: linear recurrence of order 18
 * a(n) = 10 a(n-1) + 56 a(n-2) - 828 a(n-3) + 457 a(n-4)
 *          + 18458 a(n-5) - 42836 a(n-6) - 129360 a(n-7)
 *          + 480656 a(n-8) + 107168 a(n-9) - 1616128 a(n-10)
 *          + 780160 a(n-11) + 1960960 a(n-12) - 1441792 a(n-13)
 *          - 809984 a(n-14) + 628736 a(n-15) + 122880 a(n-16)
 *          - 65536 a(n-17)
 * Initial values (offset 1):
 * a(1)=312, a(2)=1996, a(3)=12880, a(4)=88572, a(5)=609912,
 * a(6)=4361260, a(7)=30999664, a(8)=226112364, a(9)=1632434488,
 * a(10)=12023904092, a(11)=87476145488, a(12)=647317646108,
 * a(13)=4727495091576, a(14)=35056982203980, a(15)=256540627260976,
 * a(16)=1904048129719436, a(17)=13948922790405176,
 * a(18)=103557690225690108
 * The function is memoised so that an arbitrary order of calls is
 * handled efficiently in O(n) time and O(n) memory.
 */

#include <stdint.h>
#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build environment. */
/* seq_t may be a builtin unsigned integer type or an arbitrary‑precision class. */
/* idx_t is unsigned 64‑bit integer. */

static const seq_t INIT[19] = {
    0,                                 /* placeholder for index 0 */
    312ULL,                            /* a(1) */
    1996ULL,                           /* a(2) */
    12880ULL,                          /* a(3) */
    88572ULL,                          /* a(4) */
    609912ULL,                         /* a(5) */
    4361260ULL,                        /* a(6) */
    30999664ULL,                       /* a(7) */
    226112364ULL,                      /* a(8) */
    1632434488ULL,                     /* a(9) */
    12023904092ULL,                    /* a(10) */
    87476145488ULL,                    /* a(11) */
    647317646108ULL,                   /* a(12) */
    4727495091576ULL,                  /* a(13) */
    35056982203980ULL,                 /* a(14) */
    256540627260976ULL,                /* a(15) */
    1904048129719436ULL,               /* a(16) */
    13948922790405176ULL,              /* a(17) */
    103557690225690108ULL              /* a(18) */
};

static const int64_t COEFFS[19] = {
    0,          /* unused index 0 */
    10,         /* coeff for a(n-1)   */
    56,         /* coeff for a(n-2)   */
    -828,       /* coeff for a(n-3)   */
    457,        /* coeff for a(n-4)   */
    18458,      /* coeff for a(n-5)   */
    -42836,     /* coeff for a(n-6)   */
    -129360,    /* coeff for a(n-7)   */
    480656,     /* coeff for a(n-8)   */
    107168,     /* coeff for a(n-9)   */
    -1616128,   /* coeff for a(n-10)  */
    780160,     /* coeff for a(n-11)  */
    1960960,    /* coeff for a(n-12)  */
    -1441792,   /* coeff for a(n-13)  */
    -809984,    /* coeff for a(n-14)  */
    628736,     /* coeff for a(n-15)  */
    122880,     /* coeff for a(n-16)  */
    -65536      /* coeff for a(n-17)  */
};

/* Cached values for memoisation */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* allocated size of cache (number of slots) */
/* Highest index for which cache[i] is valid */
static idx_t max_computed = 0;

/* Expand the cache array to accommodate at least up to 'n' */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap)
        return;

    /* Allocate or reallocate the cache */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
    }
    /* Initialise new entries that are within the known initial region */
    for (idx_t i = cache_cap + 1; i <= n; ++i)
        cache[i] = (i <= 18) ? INIT[i] : (seq_t)0;
    cache_cap = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n < 1 */

    /* Quick return for the first 18 terms */
    if (n <= 18)
        return INIT[n];

    /* Ensure that the cache covers the requested index */
    ensure_capacity(n);

    /* Compute missing terms up to 'n' */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k <= 18) {
            cache[k] = INIT[k];
            continue;
        }
        seq_t sum = 0;
        for (int offset = 1; offset <= 17; ++offset) {
            seq_t term = cache[k - offset];
            int64_t coeff = COEFFS[offset];
            if (coeff >= 0)
                sum += term * (seq_t)coeff;
            else
                sum -= term * (seq_t)(-coeff);
        }
        cache[k] = sum;
    }
    max_computed = n;
    return cache[n];
}
