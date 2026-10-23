/*
 * A169927  Integer values of 3*n-45+360/(n/2+8) (see A169926).
 * https://oeis.org/A169927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the nth smallest integer value of 3*n-45+360/(n/2+8) for
 *            all integers n such that the expression is an integer.
 *            Equivalently, let d = n+16; then d must divide 720, and a(n) =
 *            3*d-93+720/d
 * Range:     u8 none, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses divisor enumeration of 720, deduplication
   and sorting to produce the finite ascending list.  No
   pre‑hard‑coded table of terms is used. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of the sorted distinct values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (!cache) {
        /* Enumerate all divisors of 720 */
        const uint64_t D = 720ULL;
        uint64_t divs[50];
        size_t div_cnt = 0;

        for (uint64_t i = 1; i * i <= D; ++i) {
            if (D % i == 0) {
                divs[div_cnt++] = i;
                if (i != D / i)
                    divs[div_cnt++] = D / i;
            }
        }

        /* Compute values for divisors d >= 15 (n >= -1) */
        seq_t *vals = (seq_t *)malloc(div_cnt * sizeof(seq_t));
        size_t val_cnt = 0;

        for (size_t i = 0; i < div_cnt; ++i) {
            uint64_t d = divs[i];
            if (d < 15) /* Skip n = d-16 < -1 */
                continue;

            seq_t dseq = (seq_t)d;
            seq_t val = dseq * 3 - 93 + ((seq_t)D / dseq);

            /* Deduplicate */
            bool dup = false;
            for (size_t j = 0; j < val_cnt; ++j)
                if (vals[j] == val) { dup = true; break; }
            if (!dup)
                vals[val_cnt++] = val;
        }

        /* Simple insertion sort (val_cnt <= 19) */
        for (size_t i = 1; i < val_cnt; ++i) {
            size_t j = i;
            while (j > 0 && vals[j-1] > vals[j]) {
                seq_t tmp = vals[j-1];
                vals[j-1] = vals[j];
                vals[j] = tmp;
                --j;
            }
        }

        cache = vals;
        cache_len = val_cnt;
    }

    if (n == 0 || n > cache_len)
        return (seq_t)0;  /* out of range */

    return cache[n - 1];
}
