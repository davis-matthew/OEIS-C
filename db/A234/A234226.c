/*
 * A234226  Number of (n+1) X (7+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 16*a(n-2) – 460*a(n-3) + 789*a(n-4) +
 *            7170*a(n-5) – 23944*a(n-6) – 39920*a(n-7) + 246076*a(n-8) –
 *            38440*a(n-9) – 1115136*a(n-10) + 1151040*a(n-11) +
 *            2005056*a(n-12) – 3657600*a(n-13) – 331776*a(n-14) +
 *            3456000*a(n-15) – 1658880*a(n-16); Initial values: a(1)=582144,
 *            a(2)=1123724...
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* cache[0] holds a(1) */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;           /* number of terms known (>=0) */
    static size_t cached_cap  = 0;           /* allocated capacity */

    if (n == 0)  /* undefined for n<1, return 0 */
        return 0;

    /* ensure capacity for at least n elements (1‑based) */
    size_t needed_cap = (size_t)n > 16 ? (size_t)n : 16;
    if (cached_cap < needed_cap) {
        size_t new_cap = cached_cap ? cached_cap * 2 : needed_cap;
        if (new_cap < needed_cap) new_cap = needed_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) return 0;   /* allocation failure */
        cache = tmp;
        cached_cap = new_cap;
    }

    /* initialise the first 16 terms once */
    if (cached_size < 16) {
        cache[0]  = 582144ULL;
        cache[1]  = 1123724ULL;
        cache[2]  = 2264904ULL;
        cache[3]  = 5162984ULL;
        cache[4]  = 12178344ULL;
        cache[5]  = 31418264ULL;
        cache[6]  = 83324136ULL;
        cache[7]  = 236748824ULL;
        cache[8]  = 690049704ULL;
        cache[9]  = 2128516184ULL;
        cache[10] = 6731960424ULL;
        cache[11] = 22318060184ULL;
        cache[12] = 75732130344ULL;
        cache[13] = 266822847704ULL;
        cache[14] = 958417428456ULL;
        cache[15] = 3541285003544ULL;
        cached_size = 16;
    }

    /* compute further terms up to n */
    for (idx_t i = cached_size; i < n; ++i) {
        seq_t val =
            10 * cache[i-1] + 16 * cache[i-2] - 460 * cache[i-3] + 789 * cache[i-4]
          + 7170 * cache[i-5] - 23944 * cache[i-6] - 39920 * cache[i-7]
          + 246076 * cache[i-8] - 38440 * cache[i-9] - 1115136 * cache[i-10]
          + 1151040 * cache[i-11] + 2005056 * cache[i-12]
          - 3657600 * cache[i-13] - 331776 * cache[i-14]
          + 3456000 * cache[i-15] - 1658880 * cache[i-16];
        cache[i] = val;
    }
    if (cached_size < n) cached_size = n;

    return cache[n-1];
}
