/*
 * A224155  Number of nX5 0..1 arrays with diagonals and rows unimodal and
 *          antidiagonals nondecreasing.
 * https://oeis.org/A224155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the empirical recurrence (valid for n>22) a(n)
 *            = 6*a(n-1) -9*a(n-2) -a(n-3) +a(n-4) +23*a(n-5) -38*a(n-6)
 *            +42*a(n-7) -41*a(n-8) +89*a(n-9) -67*a(n-10) -9*a(n-11)
 *            +5*a(n-12) -32*a(n-13) -50*a(n-14) -12*a(n-15) -58*a(n-16)
 *            -28*a(n-17) +12*a(n-18) +4*a(n-20) (coefficients for a(n-19...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms (1‑based index) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently stored */
    static idx_t max_computed = 0; /* largest n for which cache[n] is valid */

    /* ensure cache is at least large enough for the base terms */
    if (max_computed < 23) {
        /* allocate storage for indices 0..23 inclusive */
        size_t needed = 24; /* we ignore index 0 */
        if (!cache || cache_size < needed) {
            cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            cache_size = needed;
        }
        /* initialize the known terms; only indices 1..23 are meaningful */
        if (max_computed < 1) cache[0] = 0;          /* unused */
        if (max_computed < 1) cache[1] = (seq_t)16;
        if (max_computed < 2) cache[2] = (seq_t)101;
        if (max_computed < 3) cache[3] = (seq_t)373;
        if (max_computed < 4) cache[4] = (seq_t)1156;
        if (max_computed < 5) cache[5] = (seq_t)3469;
        if (max_computed < 6) cache[6] = (seq_t)10562;
        if (max_computed < 7) cache[7] = (seq_t)32910;
        if (max_computed < 8) cache[8] = (seq_t)105020;
        if (max_computed < 9) cache[9] = (seq_t)342575;
        if (max_computed < 10) cache[10] = (seq_t)1136503;
        if (max_computed < 11) cache[11] = (seq_t)3815322;
        if (max_computed < 12) cache[12] = (seq_t)12904448;
        if (max_computed < 13) cache[13] = (seq_t)43836891;
        if (max_computed < 14) cache[14] = (seq_t)149266926;
        if (max_computed < 15) cache[15] = (seq_t)508857756;
        if (max_computed < 16) cache[16] = (seq_t)1735637682;
        if (max_computed < 17) cache[17] = (seq_t)5921189882;
        if (max_computed < 18) cache[18] = (seq_t)20201365165;
        if (max_computed < 19) cache[19] = (seq_t)68920464104;
        if (max_computed < 20) cache[20] = (seq_t)235127759007;
        if (max_computed < 21) cache[21] = (seq_t)802136163910;
        if (max_computed < 22) cache[22] = (seq_t)2736424196762;
        /* the 23rd term is given for completeness; it is not used in the
           recurrence but we store it for consistency */
        if (max_computed < 23) cache[23] = (seq_t)9334965911219;
        max_computed = 23;
    }

    /* if requested value already computed, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* ensure cache has space for n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* compute subsequent terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v = 6 * cache[i - 1];
        v -= 9 * cache[i - 2];
        v -= 1 * cache[i - 3];
        v += 1 * cache[i - 4];
        v += 23 * cache[i - 5];
        v -= 38 * cache[i - 6];
        v += 42 * cache[i - 7];
        v -= 41 * cache[i - 8];
        v += 89 * cache[i - 9];
        v -= 67 * cache[i - 10];
        v -= 9 * cache[i - 11];
        v += 5 * cache[i - 12];
        v -= 32 * cache[i - 13];
        v -= 50 * cache[i - 14];
        v -= 12 * cache[i - 15];
        v -= 58 * cache[i - 16];
        v -= 28 * cache[i - 17];
        v += 12 * cache[i - 18];
        /* skip i-19 coefficient (0) */
        v += 4 * cache[i - 20];
        cache[i] = v;
    }

    max_computed = n;
    return cache[n];
}
