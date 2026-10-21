/*
 * A220923  Majority value maps: number of 2Xn binary arrays indicating the
 *          locations of corresponding elements equal to at least half of
 *          their horizontal, diagonal and antidiagonal neighbors in a random
 *          0..2 2Xn array.
 * https://oeis.org/A220923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 4*a(n-1) + 3*a(n-2) - 16*a(n-3) + 3*a(n-4) + 17*a(n-5)
 *            * - 16*a(n-6) - 8*a(n-7) + 12*a(n-8) - 12*a(n-9) + 8*a(n-10) * -
 *            8*a(n-12) + 16*a(n-13) - 16*a(n-14) - 32*a(n-15) * (recurrence
 *            holds for n ≥ 16) * * Initial values: * a(1) = 1 * a(2) = 10 *
 *            a(3) = 44 * a(4) = 163 * a(5) = 626 * a(6)...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache, 1‑based indices. */
    static seq_t *cache  = NULL;
    static size_t cap   = 0;
    static idx_t max_n  = 0;

    /* Hard‑coded first 15 terms. */
    static const seq_t base[15] = {
        (seq_t)1,          (seq_t)10,          (seq_t)44,
        (seq_t)163,        (seq_t)626,         (seq_t)2351,
        (seq_t)8950,       (seq_t)33895,       (seq_t)128696,
        (seq_t)488096,     (seq_t)1852077,     (seq_t)7026179,
        (seq_t)26657630,   (seq_t)101135949,   (seq_t)383705313
    };

    /* Return known values directly for n ≤ 15. */
    if (n <= 15) return base[n - 1];

    /* Allocate or grow cache if necessary. */
    if (!cache || n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory – caller can tolerate 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise first 15 entries if this is the first call. */
    if (max_n == 0) {
        for (idx_t i = 1; i <= 15; ++i)
            cache[i] = base[i - 1];
        max_n = 15;
    }

    /* Fill the cache up to n using the recurrence. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t t = 0;

        t += 4  * cache[i - 1];
        t += 3  * cache[i - 2];
        t -= 16 * cache[i - 3];
        t += 3  * cache[i - 4];
        t += 17 * cache[i - 5];
        t -= 16 * cache[i - 6];
        t -= 8  * cache[i - 7];
        t += 12 * cache[i - 8];
        t -= 12 * cache[i - 9];
        t += 8  * cache[i -10];
        t -= 8  * cache[i -12];
        t += 16 * cache[i -13];
        t -= 16 * cache[i -14];
        t -= 32 * cache[i -15];

        cache[i] = t;
    }
    max_n = n;

    return cache[n];
}
