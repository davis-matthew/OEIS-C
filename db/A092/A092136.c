/*
 * A092136  Number of spanning trees in S_5 x P_n.
 * https://oeis.org/A092136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 144*a(n-1) - 2640*a(n-2) + 6930*a(n-3) - 2640*a(n-4) +
 *            144*a(n-5) - a(n-6); Initial values: a(1)=1, a(2)=189,
 *            a(3)=24576, a(4)=3046869, a(5)=375175625, a(6)=46151368704
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Max n we expect to handle in practice (~10000).  The array is
       declared large enough to cover this without dynamic allocation
       and therefore works both for C and for the C++ big‑integer build. */
    static seq_t cache[10001 + 1];   /* cache[0] unused, indices start at 1 */
    static idx_t cache_len = 0;      /* largest n precomputed */

    /* Initialise only once. */
    if (cache_len == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)189;
        cache[3] = (seq_t)24576;
        cache[4] = (seq_t)3046869;
        cache[5] = (seq_t)375175625;
        cache[6] = (seq_t)46151368704;
        cache_len = 6;
    }

    if (n <= cache_len) {
        return cache[n];
    }

    /* Grow the cache up to the requested n.  This loop is O(n)
       overall, and because it only runs once per n, repeated
       calls are also O(n). */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t v = 144 * cache[i - 1];
        v -= 2640 * cache[i - 2];
        v += 6930 * cache[i - 3];
        v -= 2640 * cache[i - 4];
        v += 144 * cache[i - 5];
        v -= cache[i - 6];
        cache[i] = v;
    }
    cache_len = n;
    return cache[n];
}
