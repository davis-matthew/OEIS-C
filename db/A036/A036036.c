/*
 * A036036  Triangle read by rows in which row n lists all the parts of all
 *          reversed partitions of n, sorted first by length and then
 *          lexicographically.
 * https://oeis.org/A036036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 17815, u16 n <= 17815, u32 n <= 17815, u64 n <= 17815,
 *            u128 n <= 17815, big any n
 * Validated: standard, pass, 115 terms through n = 8600 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A036036 – Flattened parts of all reversed partitions.
    The sequence is produced by iterating m = 1, 2, …,
    generating all partitions of m in ascending order of parts
    (the reversed partitions), ordering partitions by increasing
    length and then lexicographically.
    The parts of each partition are output in their natural order.
    a(n) is the n‑th part in this flattened list (1‑based). */

#include <stddef.h>  /* for size_t */

/* Helper: recursively generate all partitions of 'n_rem' into
   exactly 'k_rem' parts, each part ≥ 'min', with global 'parts'
   array storing the current partial partition.
   'total_k' is the total length of the partition.
   'count' keeps the current index in the flattened list.
   When 'count' reaches 'target', 'answer' is set and recursion stops. */
static void gen_k_parts(idx_t n_rem,       /* remaining sum to partition */
                       idx_t k_rem,       /* remaining number of parts to fill */
                       idx_t min_val,     /* minimal allowed part */
                       idx_t pos,         /* current position in parts[] */
                       idx_t *parts,      /* array holding the partial partition */
                       size_t total_k,    /* full partition length */
                       idx_t *count,      /* current overall index */
                       idx_t target,      /* index to find (1‑based) */
                       seq_t *answer)     /* storage for the result */
{
    if (k_rem == 1) {                       /* last part is fixed */
        if (n_rem >= min_val) {
            parts[pos] = n_rem;
            /* output all parts of this partition */
            for (size_t i = 0; i < total_k; ++i) {
                ++(*count);
                if (*count == target) {
                    *answer = (seq_t)parts[i];
                    return;
                }
            }
        }
        return;
    }

    /* maximum value that the next part can take so that the
       remaining sum can be distributed to the remaining parts */
    idx_t max_val = n_rem / k_rem;
    for (idx_t v = min_val; v <= max_val; ++v) {
        parts[pos] = v;
        gen_k_parts(n_rem - v, k_rem - 1, v, pos + 1, parts,
                    total_k, count, target, answer);
        if (*count >= target)            /* early exit */
            return;
    }
}

seq_t a(idx_t target)
{
    /* large enough to hold partitions up to a few hundred parts */
    static idx_t parts[512];
    idx_t  curr_index = 0;
    seq_t answer = 0;

    for (idx_t m = 1; ; ++m) {                 /* iterate over rows m = 1,2,… */
        for (idx_t k = 1; k <= m; ++k) {       /* length of the partition */
            gen_k_parts(m, k, 1, 0, parts, (size_t)k,
                        &curr_index, target, &answer);
            if (curr_index >= target)          /* found the term */
                return answer;
        }
    }
    /* unreachable, but return 0 to silence compiler warnings */
    return answer;
}
