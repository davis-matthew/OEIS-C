/*
 * A025078  a(n) = s(1)s(n) + s(2)s(n-1) + ... + s(k)s(n-k+1), where k = [ n/2
 *          ] and s = (Fibonacci numbers).
 * https://oeis.org/A025078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{⌊(n-1)/2⌋} s(i)·s(n‑i), * where s(0)=1, s(1)=1
 *            and s(k)=s(k-1)+s(k-2) for k≥2 (Fibonacci)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"   /* seq_realloc */

static seq_t *fib_cache      = NULL;   /* fib_cache[0] = 1, fib_cache[1] = 1, … */
static idx_t  fib_cache_cap  = 0;      /* allocated size (maximum index + 1) */

/* Ensure fib_cache contains s(0)…s(n) */
static void ensure_fib_cache(idx_t n)
{
    /* Need indices up to n (inclusive). */
    idx_t needed = n + 1;  /* because index n may be accessed */

    if (needed > fib_cache_cap) {
        size_t new_cap = fib_cache_cap ? fib_cache_cap * 2 : 2;
        while (new_cap < (size_t)needed)
            new_cap *= 2;

        fib_cache = seq_realloc(fib_cache, fib_cache_cap, new_cap);
        /* Zero any new elements (seq_realloc does **not* guarantee zero). */
        for (idx_t i = fib_cache_cap; i < new_cap; ++i)
            fib_cache[i] = 0;

        fib_cache_cap = new_cap;
    }

    /* Initialise the first two Fibonacci numbers if not yet set. */
    if (fib_cache[0] == 0) fib_cache[0] = 1;
    if (fib_cache[1] == 0) fib_cache[1] = 1;

    /* Compute missing Fibonacci values up to 'needed'. */
    for (idx_t i = 2; i < needed; ++i)
        if (fib_cache[i] == 0)
            fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* unused – offset starts at 1 */

    /* k = ⌊(n-1)/2⌋, number of terms minus one. */
    idx_t k = (n - 1) / 2;

    ensure_fib_cache(n);                       /* s(0)…s(n) ready */

    seq_t sum = 0;
    for (idx_t i = 0; i <= k; ++i) {
        seq_t left  = fib_cache[i];
        seq_t right = fib_cache[n - i];
        sum += left * right;
    }
    return sum;
}
