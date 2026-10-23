/*
 * A139373  Let the binary expansion of n be n = Sum_{k} 2^{r_k}, let e(n) be
 *          the number of r_k's that are even, o(n) the number that are odd;
 *          sequence lists n such that e(n) > o(n).
 * https://oeis.org/A139373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer whose binary representation has
 *            a larger number of set bits in even positions (0‑based) than in
 *            odd positions. The sequence is computed by brute‑force search
 *            and memoised for arbitrary calling order
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Portable popcount for 64‑bit numbers */
static inline unsigned int popcnt_u64 (uint64_t x)
{
#ifdef __GNUC__
    return __builtin_popcountll (x);
#else
    unsigned int c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
#endif
}

/* Memoised cache of sequence terms.  */
static seq_t *seq_cache   = NULL;
static size_t   cache_len = 0;   /* number of terms stored */
static size_t   cache_cap = 0;   /* allocated capacity     */
static uint64_t last_checked = 0;/* largest integer tested */

/* Return the nth term of the sequence (1‑based). */
seq_t a (idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe at zero */

    while (cache_len < (size_t)n)
    {
        uint64_t cand = ++last_checked;   /* test next integer */

        /* Count bits in even positions (0‑based). 0x5555… has 1s on even bits. */
        const uint64_t even_mask = 0x5555555555555555ULL;
        unsigned int e = popcnt_u64 (cand & even_mask);
        unsigned int o = popcnt_u64 (cand) - e;

        if (e > o) /* qualifies for the sequence */
        {
            if (cache_len >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
                seq_t *tmp = (seq_t *)realloc (seq_cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) exit (1);            /* allocation failure */
                seq_cache = tmp;
                cache_cap = new_cap;
            }
            seq_cache[cache_len++] = (seq_t)cand;
        }
    }

    return seq_cache[(size_t)n - 1];
}
