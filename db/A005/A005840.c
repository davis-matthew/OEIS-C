/*
 * A005840  Expansion of (1-x)*e^x/(2-e^x).
 * https://oeis.org/A005840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1-n) + Sum_{k=0..n-1} binomial(n, k) * a(k), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;   /* cache[i] = a(i) for i < done */
static seq_t *row = NULL;     /* row[k] = binomial(done-1, k) */
static idx_t cap = 0;
static idx_t done = 0;

seq_t a(idx_t n) {
    if (n < done) return cache[n];

    if (n + 1 > cap) {
        idx_t new_cap = (n + 1) * 2;
        if (new_cap < 16) new_cap = 16;
        seq_t *nc = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *nr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 0; i < new_cap; i++) {
            nc[i] = 0;
            nr[i] = 0;
        }
        for (idx_t i = 0; i < cap; i++) {
            nc[i] = cache[i];
            nr[i] = row[i];
        }
        free(cache);
        free(row);
        cache = nc;
        row = nr;
        cap = new_cap;
    }

    if (done == 0) {
        cache[0] = 1;
        row[0] = 1;
        done = 1;
    }

    for (idx_t i = done; i <= n; i++) {
        /* advance Pascal row from binomial(i-1, .) to binomial(i, .) */
        row[i] = 0;
        for (idx_t k = i; k >= 1; k--) {
            row[k] += row[k - 1];
        }
        seq_t sum = 0;
        for (idx_t k = 0; k < i; k++) {
            sum += row[k] * cache[k];
        }
        cache[i] = sum + 1 - (seq_t)i;
        done = i + 1;
    }

    return cache[n];
}
