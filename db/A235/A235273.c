/*
 * A235273  Number of (n+1) X (3+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 *   OEIS A235273 – Number of (n+1)×(3+1) 0..4 arrays with every 2×2 subblock
 *   having its diagonal sum differing from its antidiagonal sum by 5.
 *
 *   Empirical recurrence (valid for n ≥ 10):
 *       a(n) = a(n‑1) + 15·a(n‑2) – 15·a(n‑3) – 80·a(n‑4)
 *             + 80·a(n‑5) + 180·a(n‑6) – 180·a(n‑7)
 *             – 144·a(n‑8) + 144·a(n‑9)
 *
 *   Initial values (n = 1 … 9):
 *        208, 380, 632, 1280, 2344, 5096, 10088, 23000, 48328
 *
 *   The function is memoised and works for calls in any order.
 *********************************************************************/

#include <stddef.h>
#include <stdlib.h>

/*  Maximum n that the function can compute directly.  The requirement
 *  specifies correct values up to about 10 000, so 10 000 is sufficient.
 *  If a larger n is requested the function abort()s – this protects
 *  against out‑of‑bounds array access.  The caller may choose to change
 *  the limit if needed. */
#define MAX_N 10000U

/*  Cache of previously computed terms.  cache[0] holds a(1), cache[1]
 *  holds a(2), etc.  The array is static so that values persist
 *  across function calls. */
static seq_t cache[MAX_N + 1] = {0};
static size_t stored = 0;   /* number of terms actually computed */

/*--------------------------------------------------------------------
 *  a(idx_t n)
 *
 *  Returns the nth term of A235273.  The function is safe for any
 *  call order and is memoised.  For n < 1 the value 0 is returned.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0;

    if (n > MAX_N) {
        /*  The cache does not cover this n; abort rather than
         *  corrupting the array.  In a real application this
         *  could be replaced by an error return or a dynamic
         *  extension of the cache. */
        abort();
    }

    /*  Initialise the first nine terms on first use. */
    if (stored == 0) {
        static const seq_t base[9] = {
            (seq_t)208,
            (seq_t)380,
            (seq_t)632,
            (seq_t)1280,
            (seq_t)2344,
            (seq_t)5096,
            (seq_t)10088,
            (seq_t)23000,
            (seq_t)48328
        };
        size_t i;
        for (i = 0; i < 9 && i < MAX_N; ++i)
            cache[i] = base[i];
        stored = (i < MAX_N ? i : MAX_N);
    }

    /*  Compute missing terms up to n.  The cache already contains
     *  values up to a(stored).  We compute a(stored+1), a(stored+2),
     *  … , a(n). */
    if (stored < (size_t)n) {
        size_t i;
        for (i = stored; i < (size_t)n; ++i) {
            /*  i is the zero‑based index of a(i+1).  We need
             *  a(i), a(i-1), … , a(i-8).  Since stored >= 9,
             *  all these indices are valid. */
            seq_t term = cache[i - 1];
            term += (seq_t)15 * cache[i - 2];
            term -= (seq_t)15 * cache[i - 3];
            term -= (seq_t)80 * cache[i - 4];
            term += (seq_t)80 * cache[i - 5];
            term += (seq_t)180 * cache[i - 6];
            term -= (seq_t)180 * cache[i - 7];
            term -= (seq_t)144 * cache[i - 8];
            term += (seq_t)144 * cache[i - 9];
            cache[i] = term;
        }
        stored = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
