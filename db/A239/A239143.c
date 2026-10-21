/*
 * A239143  Number of strict partitions of n having standard deviation σ >= 1.
 * https://oeis.org/A239143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_distinct(n) – (1 + [n odd and n ≥3] + [n divisible by 3
 *            and n ≥6])
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_distinct(n) is the number of strict (distinct‑part) partitions of n. */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *part_distinct = NULL;   /* dp array: part_distinct[k] = number of strict partitions of k */
static idx_t  max_computed = 0;       /* highest n for which part_distinct is valid */

/* Recompute distinct partitions up to n (inclusive). */
static void
ensure_partitions(idx_t n)
{
    if (n <= max_computed)
        return;                     /* already up to date */

    /* Reallocate the dp array for the new limit */
    seq_free_array(part_distinct);
    part_distinct = seq_alloc_new_array(n + 1);
    for (idx_t i = 0; i <= n; ++i)
        part_distinct[i] = 0;
    part_distinct[0] = (seq_t)1;

    /* Standard DP for partitions into distinct parts:
       iterate over each possible part value once, using reverse sum loop. */
    for (idx_t part = 1; part <= n; ++part) {
        int p = (int)part;              /* n is ≤ 10000, fits in int safely */
        for (int sum = (int)n; sum >= p; --sum) {
            part_distinct[sum] += part_distinct[sum - p];
        }
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    /* Compute distinct partitions up to n if needed */
    ensure_partitions(n);

    /* Count strict partitions with standard deviation < 1 */
    seq_t subtract = (seq_t)1;           /* the single‑part partition n itself */
    if ((n & 1) && n >= 3)               /* odd n ≥ 3 gives one consecutive 2‑part partition */
        subtract += (seq_t)1;
    if ((n % 3 == 0) && n >= 6)          /* n divisible by 3 and ≥ 6 gives one consecutive 3‑part partition */
        subtract += (seq_t)1;

    /* Result: all strict partitions – those with σ < 1 */
    return part_distinct[n] - subtract;
}
