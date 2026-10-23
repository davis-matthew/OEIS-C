/*
 * A041946  Numerators of continued fraction convergents to sqrt(496).
 * https://oeis.org/A041946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 19, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued‑fraction convergents to sqrt(496).
   Recurrence:  p_{-1}=1, p_0=22.
   For n>=1 the next partial quotient a_n is obtained by the
   standard square‑root continued‑fraction algorithm
   (m_{k+1}=d_k*a_k−m_k, d_{k+1}=(D−m_{k+1}²)/d_k,
   a_{k+1}=⌊(22+m_{k+1})/d_{k+1}⌋   with D=496 and a_0=22).
   Then   p_n = a_n * p_{n-1} + p_{n-2}. */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static state ------------------------------------------------- */
    static int initialized = 0;
    static seq_t *cache = NULL;       /* array of numerators */
    static size_t cache_cap = 0;      /* allocated capacity */
    static size_t cache_size = 0;     /* number of terms stored */

    /* Continued‑fraction parameters for sqrt(496) ------------------ */
    static const unsigned long long D  = 496ULL;   /* D in sqrt(D)  */
    static unsigned long long m = 0;               /* previous m    */
    static unsigned long long d = 1;               /* previous d    */
    static const unsigned long long a0 = 22ULL;    /* a_0           */
    static unsigned long long a_prev = a0;         /* a_{k-1}       */

    /* Ensure the cache is ready for new terms --------------------- */
    if (!initialized) {
        /* Allocate initial capacity and store a_0 */
        grow_seq(&cache, &cache_cap, 1);
        cache[0] = (seq_t)a0;
        cache_size = 1;
        initialized = 1;
    }

    /* If the requested index is already known, return it --------- */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Expand the cache up to index n ------------------------------ */
    grow_seq(&cache, &cache_cap, (size_t)n + 1);

    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* Compute next continued fraction term a_i ---------------- */
        m = d * a_prev - m;
        d = (D - m * m) / d;
        unsigned long long a_curr = (a0 + m) / d;

        /* Compute the numerator p_i ------------------------------ */
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = (i >= 2) ? cache[i - 2] : (seq_t)1;
        cache[i] = (seq_t)a_curr * prev1 + prev2;

        /* Prepare for the next iteration ------------------------- */
        a_prev = a_curr;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
