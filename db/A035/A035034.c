/*
 * A035034  Numbers k such that k >= d(k)^2, where d(k) is the number of
 *          divisors of k.
 * https://oeis.org/A035034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ n such that k ≥ d(k)^2. The implementation
 *            enumerates numbers up to a fixed bound and keeps those that
 *            satisfy the inequality. For n ≥ 1209 the closed form a(n) = n +
 *            52 (from the OEIS entry) is used
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For large n use the proven linear part of the sequence */
    if (n >= 1209) {
        return (seq_t)(n + 52);
    }

    /* Lazily generate the sequence for n < 1209 */
    static seq_t *seq = NULL;      /* storage of found k's */
    static idx_t  seq_len = 0;     /* how many are stored */
    static int    ready   = 0;     /* whether the table is built */

    if (!ready) {
        const idx_t LIMIT = 20000;          /* enough to cover all needed terms */
        /* divisor counts for 1..LIMIT */
        int *divcnt = (int*)calloc((size_t)LIMIT + 1, sizeof(int));
        for (idx_t i = 1; i <= LIMIT; ++i) {
            for (idx_t j = i; j <= LIMIT; j += i) {
                ++divcnt[j];
            }
        }
        /* allocate room for at most LIMIT terms */
        seq = (seq_t*)malloc((size_t)LIMIT * sizeof(seq_t));

        idx_t idx = 0;
        for (idx_t k = 1; k <= LIMIT; ++k) {
            seq_t kk   = (seq_t)k;
            seq_t d2   = (seq_t)divcnt[k] * (seq_t)divcnt[k];
            if (kk >= d2) {
                seq[idx++] = kk;
                if (idx >= LIMIT) break;   /* safety guard */
            }
        }
        seq_len = idx;
        free(divcnt);
        ready = 1;
    }

    /* Sequence indices are 1‑based */
    return seq[n - 1];
}
