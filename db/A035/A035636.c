/*
 * A035636  Number of partitions of n into parts 5k+3 and 5k+4 with at least
 *          one part of each type.
 * https://oeis.org/A035636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #partitions of n into parts of the form 5k+3 and 5k+4,
 *            with at least one part of each type. This is obtained from the
 *            generating function (-1+1/∏(1-x^(5k+3)))·(-1+1/∏(1-x^(5k+4)))
 *            and inclusion–exclusion: a(n) = tot(n) - only3(n) - only4(n)
 *            where tot is the number of partitions using *any*...
 * Range:     u8 n <= 50, u16 n <= 120, u32 n <= 338, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

/* static state for memoisation */
static seq_t *total = NULL;
static seq_t *only3 = NULL;
static seq_t *only4 = NULL;
static idx_t current_max = 0;
static bool initialized = false;

/* recompute tables up to n (inclusive) from scratch */
static void recompute(idx_t n)
{
    /* free previous tables if any */
    if (initialized) {
        seq_free_array(total);
        seq_free_array(only3);
        seq_free_array(only4);
    }

    /* allocate new tables of size n+1 */
    size_t sz = (size_t)n + 1;
    total = alloc_seq_array(sz);
    only3 = alloc_seq_array(sz);
    only4 = alloc_seq_array(sz);

    /* initialise all entries to zero */
    for (idx_t i = 0; i < sz; ++i) {
        total[i]  = 0;
        only3[i]  = 0;
        only4[i]  = 0;
    }
    /* base case */
    total[0] = only3[0] = only4[0] = 1;

    /* dynamic programming over available part sizes */
    for (idx_t v = 1; v <= n; ++v) {
        switch (v % 5) {
            case 3:
                for (idx_t s = v; s <= n; ++s) {
                    total[s] += total[s - v];
                    only3[s] += only3[s - v];
                }
                break;
            case 4:
                for (idx_t s = v; s <= n; ++s) {
                    total[s] += total[s - v];
                    only4[s] += only4[s - v];
                }
                break;
            default:
                /* parts of other residue classes are ignored */
                break;
        }
    }

    current_max = n;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1 – guard against 0 */
    if (!initialized || n > current_max)
        recompute(n);

    /* inclusion–exclusion: total partitions minus those using only one type */
    seq_t res = total[n];
    res -= only3[n];
    res -= only4[n];
    return res;
}
