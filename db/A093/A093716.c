/*
 * A093716  Hook products of all partitions of 5.
 * https://oeis.org/A093716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 5! / A003869(8 - n)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   cmp_seq_t, generate_partitions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: compute all hook products of the 7 partitions of 5,
   sort them in non‑decreasing order and return the n‑th element. */

#include <stdlib.h>
#include "../utils/cmp_seq_t.h"
#include "../utils/generate_partitions.h"

static int cmp_seq(const void *p1, const void *p2)
{
    return cmp_seq_t(p1, p2);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for 0 or negative indices */

    static seq_t sorted[10];
    static size_t count = 0;
    if (count == 0) 
    {
        /* build all partitions of 5 */
        int parts[10];
        seq_t hooks[10];
        size_t hooks_cnt = 0;

        generate_partitions(5, 5, parts, 0, hooks, &hooks_cnt);

        /* sort hook products */
        qsort(hooks, hooks_cnt, sizeof(seq_t), cmp_seq);
        for (size_t i = 0; i < hooks_cnt; i++)
            sorted[i] = hooks[i];
        count = hooks_cnt;   /* should be 7 */
    }

    if ((size_t)n > count)
        return (seq_t)0;   /* undefined beyond the known terms */

    return sorted[n - 1];
}
