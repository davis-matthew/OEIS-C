/*
 * A121805  The "comma sequence": the lexicographically earliest sequence of
 *          positive numbers with the property that the sequence formed by the
 *          pairs of digits adjacent to the commas between the terms is the
 *          same as the sequence of successive differences between the terms.
 * https://oeis.org/A121805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, nice
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1293, u32 n <= 7750, u64 n <= 7750,
 *            u128 n <= 7750, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implementation of OEIS sequence A121805.
 * Recurrence: a(1)=1.
 * For n>1, let x be the least‑significant digit of a(n-1);
 *   a(n) = a(n-1) + 10*x + y,
 *   where y is the most‑significant digit of a(n) and is the smallest
 *   digit 0..9 satisfying this property.  If no such y exists the
 *   sequence stops.  The memoised routine works for any order of calls.
 *
 * The function keeps a static cache so that repeated calls are answered
 * in O(1) after the first computation up to the requested index.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache (index 1 contains a(1)) */
static idx_t max_computed = 0;

/* Return the most significant decimal digit of x.  Works for all
 * widths of seq_t including arbitrary‑precision types. */
static inline seq_t msd_seq(seq_t x)
{
    while (x >= 10) x /= 10;
    return x;
}

/* Compute the n‑th term of A121805.  */
seq_t a(idx_t n)
{
    if (n == 0) { /* undefined, but return 0 for safety */
        return (seq_t)0;
    }
    /* Allocate or expand the cache as needed */
    if (n > max_computed) {
        idx_t new_size = n + 1;          /* indices 0..n, ignore 0 */
        cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!cache) {                     /* Out of memory; abort */
            return (seq_t)0;
        }
        /* Initialise a(1) if this is the first computation */
        if (max_computed == 0) {
            cache[1] = (seq_t)1;
            max_computed = 1;
        }
        /* Compute missing terms up to n */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t lsd = prev % (seq_t)10;
            seq_t base = prev + lsd * (seq_t)10;
            seq_t val = 0;
            for (seq_t d = 0; d <= 9; ++d) {
                seq_t cand = base + d;
                if (msd_seq(cand) == d) {
                    val = cand;
                    break;
                }
            }
            /* If no digit found the sequence terminates */
            if (val == 0)
                cache[i] = 0;
            else
                cache[i] = val;
        }
        max_computed = (max_computed > n) ? max_computed : n;
    }
    return cache[n];
}
