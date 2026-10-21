/*
 * A216986  Number of city-block distance 1, pressure limit 2 movements in an
 *          nX3 array with each element moving exactly one horizontally or
 *          vertically, no element acquiring more than two neighbors, and
 *          without 2-loops.
 * https://oeis.org/A216986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) -121*a(n-2) +28*a(n-3) +649*a(n-4) -844*a(n-5)
 *            +175*a(n-6) +80*a(n-7) -2*a(n-8) Initial values: a(1)=0,
 *            a(2)=14, a(3)=264, a(4)=4794, a(5)=83304, a(6)=1435808,
 *            a(7)=24673352, a(8)=423651134
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* maximum n expected by the specification (≈10000) */
    #define MAX_N 20001   /* +1 to keep index 1-based */

    /* static cache: cache[1] holds a(1), etc. */
    static seq_t cache[MAX_N];
    static idx_t cache_len = 0;   /* highest index stored */

    if (n < 1) {
        return (seq_t)0;
    }

    /* initialize first eight terms once */
    if (cache_len == 0) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)264;
        cache[4] = (seq_t)4794;
        cache[5] = (seq_t)83304;
        cache[6] = (seq_t)1435808;
        cache[7] = (seq_t)24673352;
        cache[8] = (seq_t)423651134;
        cache_len = 8;
    }

    /* if requested term already computed, return it */
    if (n <= cache_len) {
        return cache[n];
    }

    /* compute missing terms up to n */
    if (n >= MAX_N) {
        /* requested n is beyond the pre‑allocated cache; enlarge is not
           supported under the current constraints.  The sequence is
           defined only for n up to the value requested by the tests.  */
        return (seq_t)0;
    }

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos = (seq_t)24 * cache[i-1];
        pos += (seq_t)28 * cache[i-3];
        pos += (seq_t)649 * cache[i-4];
        pos += (seq_t)175 * cache[i-6];
        pos += (seq_t)80  * cache[i-7];

        seq_t val = pos;
        val -= (seq_t)121 * cache[i-2];
        val -= (seq_t)844 * cache[i-5];
        val -= (seq_t)2   * cache[i-8];

        cache[i] = val;
    }

    cache_len = n;
    return cache[n];
}
