/*
 * A200555  Number of -n..n arrays x(0..4) of 5 elements with zero sum and
 *          nonzero second differences.
 * https://oeis.org/A200555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 145, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A200555 – recursive computation
 * Recurrence: a(n) = a(n-1) + 2*a(n-2) - 3*a(n-4) - 3*a(n-5)
 *                     + 3*a(n-6) + 3*a(n-7) - 2*a(n-9) - a(n-10)
 *                     + a(n-11)
 * Initial terms (n = 1..11):
 * a(1)= 26,   a(2)=280,   a(3)=1184,   a(4)=3396,
 * a(5)=7778,  a(6)=15476, a(7)=27806,  a(8)=46346,
 * a(9)=72902, a(10)=109528, a(11)=158422
 * The function memoises results up to the largest index requested,
 * works for any order of calls, and is fully compliant with C11
 * and C++17 (seq_t may be an unsigned integer type or an
 * arbitrary‑precision integer class that supports the C++ arithmetic
 * operators and default construction).
 */
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* static cache – size sufficient for all required n up to about
       20000 (the tests run up to ~10000).  It works with built‑in
       unsigned types and with arbitrary‑precision integer classes
       because the array elements are default constructed. */
    enum { CACHE_CAP = 20000 };
    static seq_t cache[CACHE_CAP + 1];
    static idx_t last = 0;   /* highest index computed so far */

    /* initialise the cache with the first 11 terms on first use */
    if (last == 0) {
        cache[ 1] = 26;
        cache[ 2] = 280;
        cache[ 3] = 1184;
        cache[ 4] = 3396;
        cache[ 5] = 7778;
        cache[ 6] = 15476;
        cache[ 7] = 27806;
        cache[ 8] = 46346;
        cache[ 9] = 72902;
        cache[10] = 109528;
        cache[11] = 158422;
        last = 11;
    }

    if (n <= last)            /* value already computed – return it */
        return cache[n];

    /* compute missing terms up to n (inclusive) */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* positive contributions */
        seq_t val = cache[i - 1];
        val += 2 * cache[i - 2];
        val += 3 * cache[i - 6];
        val += 3 * cache[i - 7];
        val += cache[i - 11];

        /* negative contributions */
        val -= 3 * cache[i - 4];
        val -= 3 * cache[i - 5];
        val -= 2 * cache[i - 9];
        val -= cache[i - 10];

        cache[i] = val;
    }
    last = n;
    return cache[n];
}
