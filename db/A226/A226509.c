/*
 * A226509  Expansion of (3-2*x)/(1-x-x^3)+x/(1-x)^2+x/(1-x^2).
 * https://oeis.org/A226509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Let b(0)=b(1)=b(2)=1, b(k)=b(k-1)+b(k-3) for k≥3. Then a(n) =
 *            3*b(n) – 2*b(n-1) + n + (n mod 2)
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 58, u64 n <= 116,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 3*b(n) - 2*b(n-1) + n + (n & 1) */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *b_cache = NULL;   /* cache for b(k) values */
static idx_t  b_len   = 0;       /* number of cached elements (next free index) */

/* Ensure that b_cache[0 .. n] exists and are computed. */
static void compute_b_to(idx_t n)
{
    if (b_len <= n) {
        size_t newsize = (size_t)(n + 1);
        if (b_cache == NULL) {
            b_cache = (seq_t*)malloc(newsize * sizeof(seq_t));
            if (!b_cache) abort();
        } else {
            seq_t *tmp = (seq_t*)realloc(b_cache, newsize * sizeof(seq_t));
            if (!tmp) abort();
            b_cache = tmp;
        }

        for (idx_t i = b_len; i <= n; ++i) {
            if (i <= 2) {
                b_cache[i] = (seq_t)1;
            } else {
                b_cache[i] = b_cache[i - 1] + b_cache[i - 3];
            }
        }
        b_len = n + 1;   /* we now have indices 0 .. n */
    }
}

seq_t a(idx_t n)
{
    compute_b_to(n);

    seq_t bn  = b_cache[n];
    seq_t bn1 = (n == 0) ? (seq_t)0 : b_cache[n - 1];

    seq_t result = (seq_t)3 * bn - (seq_t)2 * bn1 + (seq_t)n + (seq_t)(n & 1u);
    return result;
}
