/*
 * A168682  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 5, a(2) = 20, … . Recurrence (derived from the
 *            rational generating function) For all n ≥ 0 a(n) = 4·a(n−1) –
 *            9·a(n−17) + 6·a(n−18) + pₙ where the boundary terms a(k)=0 for
 *            k<0 and p₀ = 1, p₁ = 1, p₁₇ = –1, p₁₈ = –1, all other pₙ = 0.
 *            The recurrence uses only additions, subtractio...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard allocation functions; the cast is required for C++ */
#include <stdlib.h>

/*  The sequence calculator.  It uses a static memoised cache so that
    calls may occur in any order. */
seq_t a(idx_t n)
{
    /*  Cache of already computed terms.  The array is grown
        as needed and never shrinks.  cache[0] is the first term.  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /*  Initialise cache with the first known term.  */
    if (cache == NULL) {
        cache = (seq_t *)calloc(1, sizeof(seq_t));   /* allocate a[0]   */
        if (cache == NULL) return (seq_t)0;          /* out of memory   */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /*  Extend cache if the requested index is beyond the current size. */
    if (n >= cache_size) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* out of memory   */

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = 0;

            if (i >= 1)          /* 4·a(i‑1)  */
                val += cache[i - 1] * (seq_t)4;
            if (i >= 17)         /* –9·a(i‑17)  */
                val -= cache[i - 17] * (seq_t)9;
            if (i >= 18)         /* +6·a(i‑18)  */
                val += cache[i - 18] * (seq_t)6;

            /*  Add the constant term pᵢ.  */
            if (i == 0 || i == 1)       /* p₀ = 1, p₁ = 1   */
                val += (seq_t)1;
            else if (i == 17 || i == 18)   /* p₁₇ = –1, p₁₈ = –1 */
                val -= (seq_t)1;

            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
