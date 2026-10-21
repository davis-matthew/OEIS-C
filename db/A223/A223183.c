/*
 * A223183  Rolling icosahedron footprints: number of nX5 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal, vertical or antidiagonal
 *          neighbor moves along an icosahedral edge.
 * https://oeis.org/A223183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence for n>14 * a(n) = 18*a(n-1) -114*a(n-2) +436*a(n-3)
 *            -1171*a(n-4) +2198*a(n-5) * -2626*a(n-6) +1520*a(n-7) +79*a(n-8)
 *            -490*a(n-9) +156*a(n-10) * +20*a(n-11) -16*a(n-12) * with
 *            initial values a(1)=625, a(2)=1280, a(3)=10880, a(4)=103520, *
 *            a(5)=1018720, a(6)=10117600, a(7)=100734400, a(8...
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* The maximum value of n that will be requested in the tests */
    /* Adjust if a larger bound is desired (e.g. 20000) */
    enum { MAX_N = 10000 };

    /* Stored initial values (index 0 unused) */
    static const seq_t init[15] = {
        /*0*/ 0,
        /*1*/ 625,
        /*2*/ 1280,
        /*3*/ 10880,
        /*4*/ 103520,
        /*5*/ 1018720,
        /*6*/ 10117600,
        /*7*/ 100734400,
        /*8*/ 1003652480,
        /*9*/ 10001217120,
        /*10*/ 99661921440,
        /*11*/ 993129226400,
        /*12*/ 9896517562400,
        /*13*/ 98618666649920,
        /*14*/ 982733746858880
    };

    /* Static cache large enough for all required n */
    static seq_t cache[MAX_N + 1];
    static idx_t cache_max = 0;      /* largest n for which cache is valid */

    /* Initialise the cache on the first call */
    if (cache_max == 0) {
        /* copy initial values */
        for (idx_t i = 1; i <= 14 && i <= (idx_t)MAX_N; ++i) {
            cache[i]   = init[i];
        }
        /* remaining entries are implicitly zero-initialised by static storage */
        cache_max = 14;
    }

    /* If the requested n is already in the cache, return it */
    if (n <= cache_max) {
        return cache[n];
    }

    /* Compute missing terms up to n, using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val  = ((seq_t)18)   * cache[i - 1];
        val       -= ((seq_t)114)  * cache[i - 2];
        val       += ((seq_t)436)  * cache[i - 3];
        val       -= ((seq_t)1171) * cache[i - 4];
        val       += ((seq_t)2198) * cache[i - 5];
        val       -= ((seq_t)2626) * cache[i - 6];
        val       += ((seq_t)1520) * cache[i - 7];
        val       += ((seq_t)79)   * cache[i - 8];
        val       -= ((seq_t)490)  * cache[i - 9];
        val       += ((seq_t)156)  * cache[i -10];
        val       += ((seq_t)20)   * cache[i -11];
        val       -= ((seq_t)16)   * cache[i -12];
        cache[i] = val;
    }

    /* Update the cached maximum index */
    cache_max = n;

    return cache[n];
}
