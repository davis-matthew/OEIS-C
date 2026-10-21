/*
 * A218349  Number of n X 3 binary arrays with every 0 a horizontal or
 *          vertical neighbor to some 1.
 * https://oeis.org/A218349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) +5*a(n-2) +22*a(n-3) +7*a(n-4) +8*a(n-5)
 *            -18*a(n-6) -20*a(n-7) -a(n-8) +4*a(n-9) +3*a(n-10) +a(n-12),
 *            with initial values a(1)=5, a(2)=41, a(3)=291, a(4)=2069,
 *            a(5)=14811, a(6)=105913, a(7)=757305, a(8)=5415209,
 *            a(9)=38722037, a(10)=276885777, a(11)=1979899795,
 *            a(12)=14157473937
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Allocate a cache large enough for n up to 10000. */
    static seq_t cache[10001] = {0};
    /* last holds the largest index that has been computed (initialized to 0). */
    static idx_t last = 0;

    /* Initialize first 12 terms once. */
    if (last == 0) {
        const seq_t init[12] = {
            5ULL, 41ULL, 291ULL, 2069ULL, 14811ULL, 105913ULL,
            757305ULL, 5415209ULL, 38722037ULL,
            276885777ULL, 1979899795ULL, 14157473937ULL
        };
        for (idx_t i = 1; i <= 12; ++i) cache[i] = init[i - 1];
        last = 12;
    }

    if (n <= last) return cache[n];

    /* Guard against requests beyond the maximum supported index. */
    if (n > 10000) return 0;

    /* Compute missing terms up to n. */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t t = 0;
        t += 6 * cache[k - 1];
        t += 5 * cache[k - 2];
        t += 22 * cache[k - 3];
        t += 7 * cache[k - 4];
        t += 8 * cache[k - 5];
        t -= 18 * cache[k - 6];
        t -= 20 * cache[k - 7];
        t -= 1 * cache[k - 8];
        t += 4 * cache[k - 9];
        t += 3 * cache[k - 10];
        t += 1 * cache[k - 12];
        cache[k] = t;
    }

    last = n;
    return cache[n];
}
