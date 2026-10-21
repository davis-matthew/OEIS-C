/*
 * A203184  Number of n X 1 0..4 arrays with every nonzero element less than
 *          or equal to some horizontal or vertical neighbor.
 * https://oeis.org/A203184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A203184:
   a(1) = 1,  a(2) = 5,  a(3) = 25,  a(4) = 95,  a(5) = 325,
   a(6) = 1121,  a(7) = 3985,  a(8) = 14288,  a(9) = 50995.
   For n ≥ 10 the values satisfy the linear recurrence
   a(n) = 5*a(n-1) - 10*a(n-2) + 20*a(n-3) – 15*a(n-4)
          + 21*a(n-5) – 7*a(n-6) + 8*a(n-7) – a(n-8) + a(n-9);
   The sequence counts n×1 arrays with entries 0…4 such that every
   non‑zero entry is ≤ at least one of its horizontal or vertical
   neighbors. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t brute(idx_t n)
{
    idx_t total = 1;
    for (idx_t i = 0; i < n; ++i) total *= 5;
    seq_t count = 0;
    for (idx_t num = 0; num < total; ++num) {
        idx_t k = num;
        uint8_t v[9];   /* n ≤ 9 for the brute function */
        for (idx_t i = 0; i < n; ++i) {
            v[i] = (uint8_t)(k % 5);
            k   /= 5;
        }
        bool ok = true;
        for (idx_t i = 0; i < n; ++i) {
            if (v[i] == 0) continue;
            bool neigh = false;
            if (i > 0   && v[i-1] >= v[i]) neigh = true;
            if (i + 1 < n && v[i+1] >= v[i]) neigh = true;
            if (!neigh) { ok = false; break; }
        }
        if (ok) ++count;
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* stored values: a(1)…a(cache_len) */

    if (n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failure – avoid crash */
            return 0;
        }
        cache = tmp;

        /* compute new entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            idx_t idx = (idx_t)(i + 1);   /* current n (1‑based) */
            seq_t value;
            if (idx <= 9) {
                value = brute(idx);
            } else {
                static const int coeff[9] = {5, -10, 20, -15, 21, -7, 8, -1, 1};
                seq_t pos = 0;
                seq_t neg = 0;
                for (int k = 0; k < 9; ++k) {
                    seq_t t = cache[i - 1 - (size_t)k];
                    int c = coeff[k];
                    if (c >= 0)
                        pos += t * (seq_t)c;
                    else
                        neg += t * (seq_t)(-c);
                }
                value = pos - neg;
            }
            cache[i] = value;
        }
        cache_len = new_len;
    }
    return cache[n - 1];
}
