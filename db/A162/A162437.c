/*
 * A162437  a(1)=1, a(2)=2. Take terms a(n-1) and a(n-2), then convert to
 *          binary. Concatenate them, with either binary a(n-1) on the left
 *          and a(n-2) on the right, or with a(n-1) on the right and a(n-2) on
 *          the left such that the value of the resulting binary number is
 *          minimized. a(n) = the decimal equivalent...
 * https://oeis.org/A162437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1, a(2)=2; for n≥3, a(n)=min( (a(n‑1)<<len(a(n‑2)))|a(n‑2),
 *            (a(n‑2)<<len(a(n‑1)))|a(n‑1) ), where len(x) is the number of
 *            bits of the binary representation of x
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* Local bit‐length routine: works for any unsigned seq_t type, including
   built‑in integers and the arbitrary‑precision class.  The loop runs at
   most as many times as there are bits in x, which is few for the sizes
   involved up to n≈10000. */
static unsigned bitlen_seq_t_local(seq_t x)
{
    unsigned len = 0;
    while (x)
    {
        x >>= 1;
        ++len;
    }
    return len;
}

/* External linkage as required.  The function is memoised: a static
   array stores all already computed terms, so subsequent calls are O(1). */
seq_t a(idx_t n)
{
    if (n < 1)          /* undefined indices return 0 */
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;   /* highest index that has been filled */

    /* Expand cache if necessary.  cache[0] is unused. */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failure – return 0 (unlikely) */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base terms once. */
    if (computed == 0)
    {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        computed = 2;
    }

    /* Compute forward as needed, independent of call order. */
    while (computed < n)
    {
        seq_t prev1 = cache[computed];     /* a(i‑1) where i = computed+1 */
        seq_t prev2 = cache[computed-1];   /* a(i‑2)      */
        unsigned len1 = bitlen_seq_t_local(prev1);
        unsigned len2 = bitlen_seq_t_local(prev2);

        seq_t shift1 = pow2(len2);        /* 2^len2 */
        seq_t cand1  = (prev1 * shift1) | prev2;

        seq_t shift2 = pow2(len1);        /* 2^len1 */
        seq_t cand2  = (prev2 * shift2) | prev1;

        cache[computed+1] = cand1 < cand2 ? cand1 : cand2;
        ++computed;
    }

    return cache[n];
}
