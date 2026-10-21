/*
 * A079962  Number of permutations satisfying -k <= p(i) - i <= r and p(i) - i
 *          not in I, i=1..n, with k=1, r=5, I={1,3}.
 * https://oeis.org/A079962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) + a(n-6), with a(0) = a(1) =
 *            a(2) = 1 (OEIS A079962 – compositions of n using {1,3,5,6})
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static cache grows only to a safe limit (≈ 20000) – enough for
       the required n ≤ 10000.  The array is statically allocated,
       so the type seq_t is fully constructed for both C and C++. */
    enum { CACHE_CAP = 20001 };          /* indices 0 … 20000 inclusive */
    static seq_t cache[CACHE_CAP];
    static idx_t used = 0;               /* next index that needs value */

    /* initialise first three terms once */
    if (used == 0) {
        /* The underlying type seq_t is default‑initialised to zero
           for static storage.  We now set the base values. */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        used = 3;                       /* values 0,1,2 are ready */
    }

    /* compute any missing values up to n */
    if (used <= n) {
        for (idx_t i = used; i <= n; ++i) {
            seq_t v = 0;
            if (i >= 1) v += cache[i - 1];
            if (i >= 3) v += cache[i - 3];
            if (i >= 5) v += cache[i - 5];
            if (i >= 6) v += cache[i - 6];
            cache[i] = v;
        }
        used = n + 1;                    /* next free index */
    }

    return cache[n];
}
