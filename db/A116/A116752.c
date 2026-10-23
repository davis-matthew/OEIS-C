/*
 * A116752  Number of permutations of length n which avoid the patterns 1234,
 *          1342, 2431.
 * https://oeis.org/A116752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116752: Number of permutations of length n which avoid the patterns 1234, 1342, 2431.
   Recurrence (for n > 8):
     a(n) = 11 a(n-1) - 51 a(n-2) + 130 a(n-3) - 199 a(n-4)
            + 187 a(n-5) - 105 a(n-6) + 32 a(n-7) - 4 a(n-8)
   Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=74,
                   a(6)=248, a(7)=787, a(8)=2389.
*/

#include <stdlib.h>
#include <stdint.h>

static const seq_t init8[8] = {1, 2, 6, 21, 74, 248, 787, 2389};
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but return 0 */

    if (cache == NULL) {
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
    }

    if (n <= cache_len) return cache[n - 1];

    ensure_capacity((size_t)n);
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i <= 8) {
            cache[i - 1] = init8[i - 1];
        } else {
            /* compute using the recurrence with non‑negative intermediate sums */
            seq_t pos = 0, neg = 0;
            pos += cache[i - 2] * 11;   /* a(n-1) */
            pos += cache[i - 4] * 130;  /* a(n-3) */
            pos += cache[i - 6] * 187;  /* a(n-5) */
            pos += cache[i - 8] * 32;   /* a(n-7) */

            neg += cache[i - 3] * 51;   /* a(n-2) */
            neg += cache[i - 5] * 199;  /* a(n-4) */
            neg += cache[i - 7] * 105;  /* a(n-6) */
            neg += cache[i - 9] * 4;    /* a(n-8) */

            cache[i - 1] = pos - neg;
        }
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
