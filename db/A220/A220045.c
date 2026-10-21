/*
 * A220045  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and nonincreasing columns, 0..3 2 X n array.
 * https://oeis.org/A220045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 10, a(2) = 13, a(3) = 25, a(4) = 50 For n >= 5: a(n) =
 *            (n^5 - 5 n^4 + 25 n^3 + 545 n^2 - 2126 n + 4680)/120 For n > 10:
 *            a(n) = 6 a(n-1) - 15 a(n-2) + 20 a(n-3) - 15 a(n-4) + 6 a(n-5) -
 *            a(n-6)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc */

static seq_t *cache = NULL;   /* cached sequence values, 0–based */
static size_t cache_len = 0;  /* number of terms currently cached */

static seq_t poly_term(idx_t n) {
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t n5 = n4 * N;
    seq_t num = n5;
    num -= (seq_t)5 * n4;
    num += (seq_t)25 * n3;
    num += (seq_t)545 * n2;
    num -= (seq_t)2126 * N;
    num += (seq_t)4680;
    return num / (seq_t)120;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but return 0 for safety */

    size_t needed = (size_t)n;
    if (needed > cache_len) {
        /* Grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp && needed > 0) abort();   /* allocation failed */
        cache = tmp;

        /* Compute missing terms */
        for (size_t i = cache_len + 1; i <= needed; ++i) {
            if (i == 1) cache[i - 1] = (seq_t)10;
            else if (i == 2) cache[i - 1] = (seq_t)13;
            else if (i == 3) cache[i - 1] = (seq_t)25;
            else if (i == 4) cache[i - 1] = (seq_t)50;
            else if (i <= 10)
                cache[i - 1] = poly_term((idx_t)i);
            else {
                /* Recurrence for n > 10 */
                seq_t pos = cache[i - 2] * (seq_t)6
                          + cache[i - 4] * (seq_t)20
                          + cache[i - 6] * (seq_t)6;
                seq_t sub = cache[i - 3] * (seq_t)15
                          + cache[i - 5] * (seq_t)15
                          + cache[i - 7];
                cache[i - 1] = pos - sub;  /* guaranteed non‑negative */
            }
        }
        cache_len = needed;
    }
    return cache[n - 1];
}
