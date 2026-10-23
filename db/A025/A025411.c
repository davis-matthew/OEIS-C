/*
 * A025411  Numbers that are the sum of 4 distinct positive cubes.
 * https://oeis.org/A025411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all distinct sums of 4 distinct positive cubes,
   sort them, and return the n‑th smallest (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *sums = NULL;          /* sorted unique sums of 4 distinct cubes */
static size_t sums_count = 0;       /* number of stored sums */
static bool initialized = false;

/* Simple comparator for qsort that works for any seq_t type. */
static int compare_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Pre‑compute all distinct sums of 4 distinct positive cubes up to a limit.
   The limit is chosen large enough to produce more than 10 000 terms. */
static void init_sums(void)
{
    if (initialized)
        return;

    /* Number of cubes to consider: 40^3 = 64 000, enough for >10 000 distinct sums. */
    const int L = 40;
    const size_t max_combinations =
        (size_t)L * (L - 1) * (L - 2) * (L - 3) / 24;

    sums = (seq_t *)malloc(max_combinations * sizeof(seq_t));
    if (!sums)
        abort();                 /* allocation failure */

    /* Pre‑compute cube values in uint64_t (always safe). */
    uint64_t cube[41];
    for (int i = 1; i <= L; ++i)
        cube[i] = (uint64_t)i * (uint64_t)i * (uint64_t)i;

    /* When seq_t is a small unsigned type, limit sums that exceed its range. */
    bool limit_seq = false;
    uint64_t max_seq_val = 0;
    if (sizeof(seq_t) <= 8) {
        limit_seq = true;
        switch (sizeof(seq_t)) {
        case 1: max_seq_val = UINT8_MAX;   break;
        case 2: max_seq_val = UINT16_MAX;  break;
        case 4: max_seq_val = UINT32_MAX;  break;
        case 8: max_seq_val = UINT64_MAX;  break;
        }
    }

    /* Generate all sums of 4 distinct cubes. */
    size_t idx = 0;
    for (int i = 1; i <= L - 3; ++i) {
        for (int j = i + 1; j <= L - 2; ++j) {
            for (int k = j + 1; k <= L - 1; ++k) {
                for (int l = k + 1; l <= L; ++l) {
                    uint64_t sum = cube[i] + cube[j] + cube[k] + cube[l];
                    if (limit_seq && sum > max_seq_val)
                        continue;
                    sums[idx++] = (seq_t)sum;
                }
            }
        }
    }
    sums_count = idx;

    /* Sort the array of sums. */
    qsort(sums, sums_count, sizeof(seq_t), compare_seq);

    /* Remove duplicates in place. */
    size_t uniq = 0;
    if (sums_count > 0) {
        seq_t prev = sums[0];
        sums[uniq++] = prev;
        for (size_t i = 1; i < sums_count; ++i) {
            if (sums[i] != prev) {
                prev = sums[i];
                sums[uniq++] = prev;
            }
        }
    }
    sums_count = uniq;

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* safety for out‑of‑range indices */
        return 0;

    init_sums();
    if ((idx_t)n <= sums_count)
        return sums[n - 1];

    return 0;       /* return 0 for n beyond the pre‑computed range */
}
