/*
 * A230658  Number of nX3 0..2 black square subarrays x(i,j) with each element
 *          diagonally or antidiagonally next to at least one element with
 *          value 2-x(i,j).
 * https://oeis.org/A230658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-2) - 4*a(n-4) for n ≥ 5 Initial values: a(1)=0,
 *            a(2)=3, a(3)=15, a(4)=21
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based cache, index 0 unused */
static size_t cache_size = 0;        /* size of allocated cache (max usable index + 1) */

static const seq_t base[5] = {0, 0, 3, 15, 21};

/* Compute a(n) for any n ≥ 1.  Results are memoised so that repeated
   calls are O(1).  The function is thread‑invisible (per the
   requirements). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* The sequence is defined for n ≥ 1; behaviour for 0 is
           unspecified. Return 0 to stay safe. */
        return (seq_t)0;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = ((size_t)n + 1) * 2;  /* double to amortise reallocs */
        if (new_size < (size_t)n + 1) new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – abort as we cannot continue safely. */
            abort();
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values up to n */
    for (size_t i = (size_t)cache[0] ? 0 : 1; i <= (size_t)n; ++i) {
        if (i == 0) continue;                 /* index 0 unused */
        if (i <= 4) {
            cache[i] = base[i];
        } else {
            seq_t val = cache[i - 2] * (seq_t)9;
            seq_t sub = cache[i - 4] * (seq_t)4;
            cache[i] = val - sub;
        }
    }

    return cache[(size_t)n];
}
