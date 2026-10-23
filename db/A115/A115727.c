/*
 * A115727  Maximum rectangle of partitions in Mathematica order.
 * https://oeis.org/A115727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum rectangle area of the partition at index n in
 *            Mathematica order (offset 0)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   max_rectangle
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/max_rectangle.h"

/* Recursively generate all partitions of 'rem' in descending order with
   parts no larger than 'max_p', counting them in '*cnt'.
   When the 'target'‑th partition is reached, it returns its maximum
   rectangle area; otherwise it returns 0. */
static seq_t generate_partitions(int rem, int max_p, int *parts, int sz,
                                 uint64_t *cnt, idx_t target)
{
    if (rem == 0) {
        (*cnt)++;
        if (*cnt == target) {
            return max_rectangle(parts, sz);
        }
        return 0;
    }
    int limit = rem < max_p ? rem : max_p;
    for (int i = limit; i >= 1; --i) {
        parts[sz] = i;
        seq_t res = generate_partitions(rem - i, i, parts, sz + 1, cnt, target);
        if (res != 0) return res;   /* found the target partition */
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* a(0) = 0 (empty partition) */

    uint64_t counter = 0;   /* count of partitions processed so far */
    /* iterate over all integers, generating their partitions */
    for (int sum = 1; ; ++sum) {
        int parts[256];
        seq_t res = generate_partitions(sum, sum, parts, 0, &counter, n);
        if (res != 0) return res;
        /* For safety: if counter never reaches n, loop would continue.
           In normal use (n ≤ 10000) counter will reach n quickly. */
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
