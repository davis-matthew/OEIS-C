/*
 * A085642  Number of columns in the character table of the symmetric group
 *          S_n that have zero sum.
 * https://oeis.org/A085642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 128, u64 n <= 416,
 *            u128 n <= 1458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of n that contain at least one part
 * congruent to 2 (mod 4).  It is computed by a two‑state dynamic programming
 * that keeps separate counts for partitions that already contain such a part
 * and for partitions that do not yet contain one.  The DP processes the
 * parts in increasing order; adding a part that is 2 (mod 4) turns a partition
 * into the “has‑special” state, while adding a non‑special part preserves
 * the current state.
 *
 * Complexity:  O(N²) additions of seq_t numbers, where N is the maximum
 * queried n (≤ 10000).  This easily satisfies the 2 s limit.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAX_N 10000  /* sufficient for all required queries */

static seq_t has_part[MAX_N + 1];    /* partitions that have a 2 (mod 4) part */
static seq_t no_part [MAX_N + 1];    /* partitions without such a part */
static bool initialized = false;

/* one‑time precomputation of the two DP tables */
static void precompute(void)
{
    if (initialized) return;

    has_part[0] = 0;
    no_part [0] = 1;  /* empty partition has no special part */

    for (idx_t k = 1; k <= MAX_N; ++k) {
        bool special = (k % 4 == 2);  /* part congruent to 2 mod 4 */
        for (idx_t i = k; i <= MAX_N; ++i) {
            if (special) {
                /* using a special part turns the partition into “has” */
                has_part[i] += has_part[i - k] + no_part[i - k];
            } else {
                /* non‑special part preserves current state */
                has_part[i] += has_part[i - k];
                no_part [i] += no_part [i - k];
            }
        }
    }
    initialized = true;
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    if (n < 1 || n > MAX_N) return 0;  /* outside supported range */
    precompute();
    return has_part[n];
}
