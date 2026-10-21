/*
 * A217233  Expansion of (1-2*x+x^2)/(1-3*x-3*x^2+x^3).
 * https://oeis.org/A217233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A217233 – Expansion of (1-2*x+x^2)/(1-3*x-3*x^2+x^3)
 *
 * Recurrence: a(0)=1, a(1)=1,
 *              a(n) = 4*a(n-1) - a(n-2) + 4*(-1)^n   for n>=2
 *
 * The function memoises previously computed terms in a static cache,
 * allowing arbitrary‑order calls with O(1) after the first full
 * computation.  All arithmetic uses only seq_t operators, no
 * floating point, and the implementation is valid both in C11 and
 * C++17.
 */

#include <stdlib.h>

/* ------------------------------------------------------------ */
/* seq_t and idx_t are supplied by the build system.           */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL; /* pointer to array of seq_t values   */
    static idx_t cap      = 0;    /* allocated capacity (number of slots)*/
    static idx_t size     = 0;    /* number of elements computed so far   */

    /* Ensure we can index up to n */
    if (cap < n + 1) {
        idx_t new_cap = (cap == 0) ? 4 : cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed – caller will see 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base cases when required */
    if (size == 0) {                  /* first call or cache cleared */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        size     = 1;
    }
    if (size == 1 && n >= 1) {        /* a(1) = 1 */
        cache[1] = (seq_t)1;
        size     = 2;
    }

    /* Compute terms up to requested n */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0 || i == 1) continue;   /* already initialised */

        /* recurrence: a[i] = 4*a[i-1] - a[i-2] + 4*(-1)^i */
        seq_t val = cache[i-1] * (seq_t)4;
        val = val - cache[i-2];
        if ((i & 1) == 0)                    /* i even: (-1)^i = +1   */
            val = val + (seq_t)4;
        else                                 /* i odd : (-1)^i = -1   */
            val = val - (seq_t)4;

        cache[i] = val;
    }

    size = n + 1;                       /* all terms up to n are stored */
    return cache[n];
}
