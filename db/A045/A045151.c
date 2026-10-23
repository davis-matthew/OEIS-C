/*
 * A045151  Numbers whose base-4 representation contains exactly three 2's and
 *          three 3's.
 * https://oeis.org/A045151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer whose base‑4 representation contains
 *            exactly three 2's and three 3's
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static cache of precomputed values */
static seq_t *seq_cache = NULL;
static size_t seq_count = 0;
static size_t seq_capacity = 0;

/* Build the cache once for all future calls.  We examine numbers up to
   4^10 (1 048 576) which is enough to contain the first 10 000 terms. */
static void build_cache(void)
{
    if (seq_cache != NULL)
        return;

    const idx_t LIMIT = 1048576ULL;    /* 4^10 */
    seq_capacity = 30000;              /* initial capacity, will grow if necessary */
    seq_cache = (seq_t*)malloc(seq_capacity * sizeof(seq_t));
    seq_count  = 0;

    for (idx_t num = 0; num < LIMIT; ++num) {
        if (num == 0)
            continue;                 /* skip zero */

        idx_t tmp = num;
        int c2 = 0, c3 = 0;

        /* count digits 2 and 3 in base 4 */
        while (tmp != 0) {
            int d = tmp & 3;          /* tmp % 4 */
            tmp >>= 2;                /* tmp /= 4 */
            if (d == 2) ++c2;
            if (d == 3) ++c3;
        }

        if (c2 == 3 && c3 == 3) {
            if (seq_count == seq_capacity) {
                seq_capacity <<= 1;
                seq_cache = (seq_t*)realloc(seq_cache, seq_capacity * sizeof(seq_t));
            }
            seq_cache[seq_count++] = (seq_t)num;
        }
    }
}

/* Public function required by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0)   /* OEIS offset is 1; return 0 for non‑positive indices */
        return (seq_t)0;

    build_cache();

    if (n - 1 >= seq_count)          /* safety: out of range */
        return (seq_t)0;

    return seq_cache[n - 1];
}
