/*
 * A203164  (n-1)-st elementary symmetric function of the first n terms of the
 *          periodic sequence (4,1,2,3,4,1,2,3,...).
 * https://oeis.org/A203164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A203164: (n‑1)-st elementary symmetric function of the first n terms
    of the periodic sequence (4,1,2,3,4,1,2,3, ...).
    Recurrence (conjecture, holds for n≥9):
        a(n) = 48*a(n-4) – 576*a(n-8)
    Initial values:
        a(1)=1, a(2)=5, a(3)=14, a(4)=50,
        a(5)=224, a(6)=320, a(7)=736, a(8)=2400 */
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* static cache of computed terms (index 0 unused) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* ------------------------------------------------------------------ */
/*  Ensure that terms up to n are available in cache.                 */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap)            /* already computed */
        return;

    /* allocate new array of size n+1 (index 0 unused) */
    seq_t *new_cache = seq_alloc_new_array((size_t)n + 1);

    /* free previous cache if any */
    if (cache)
        seq_free_array(cache);

    cache     = new_cache;
    cache_cap = (size_t)n;

    /* fill the cache ---------------------------------------------- */
    for (idx_t i = 1; i <= n; ++i) {
        size_t idx = (size_t)i;
        switch ((int)i) {
            case 1: cache[idx] = (seq_t)1;    break;
            case 2: cache[idx] = (seq_t)5;    break;
            case 3: cache[idx] = (seq_t)14;   break;
            case 4: cache[idx] = (seq_t)50;   break;
            case 5: cache[idx] = (seq_t)224;  break;
            case 6: cache[idx] = (seq_t)320;  break;
            case 7: cache[idx] = (seq_t)736;  break;
            case 8: cache[idx] = (seq_t)2400; break;
            default:
                /* a(i) = 48*a(i-4) - 576*a(i-8) */
                seq_t t1 = (seq_t)48;
                t1 *= cache[idx - 4];
                seq_t t2 = (seq_t)576;
                t2 *= cache[idx - 8];
                cache[idx] = t1 - t2;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public function requested by the interface.                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; value for n<1 is irrelevant */
        return (seq_t)0;

    ensure_cache(n);
    return cache[(size_t)n];
}
