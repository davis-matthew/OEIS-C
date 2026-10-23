/*
 * A049893  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 3.
 * https://oeis.org/A049893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)+…+a(n‑1) – a(m) (n ≥ 4) where m = 2^(p+1) + 2 – n
 *            and p = ⌊log₂(n–2)⌋; initial values: a(1)=1, a(2)=1, a(3)=3
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_log2_uint64.h" /* floor_log2_uint64(idx_t) */
#include "../utils/pow2.h"              /* pow2(idx_t) */

seq_t a(idx_t n)
{
    /* Memoised table of values */
    static seq_t *table = NULL;
    static size_t capacity = 0;     /* allocated length of table */
    static idx_t  num     = 0;      /* number of values stored (== table[0..num‑1]) */
    static seq_t  sum     = 0;      /* sum of the first num terms */

    if (n == 0) return 0;           /* defensive: OEIS offset starts at 1 */

    /* Grow the table up to index n */
    while (num < n)
    {
        /* Ensure capacity */
        if (num == capacity)
        {
            size_t new_cap = capacity == 0 ? 1 : capacity * 2;
            seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE); /* simple error handling */
            table    = tmp;
            capacity = new_cap;
        }

        /* index of the value to compute (1‑based) */
        idx_t cur = num + 1;

        if (cur == 1)                /* a(1) */
        {
            table[num] = (seq_t)1;
            sum += table[num];
            num++;
        }
        else if (cur == 2)            /* a(2) */
        {
            table[num] = (seq_t)1;
            sum += table[num];
            num++;
        }
        else if (cur == 3)            /* a(3) */
        {
            table[num] = (seq_t)3;
            sum += table[num];
            num++;
        }
        else                          /* n ≥ 4 */
        {
            /* p = floor_log2(cur-2) */
            idx_t p = floor_log2_uint64(cur - 2);

            /* 2^(p+1) as seq_t */
            seq_t pow_val = pow2(p + 1);

            /* m = 2^(p+1) + 2 – cur   (must fit in idx_t) */
            seq_t m_seq = pow_val + (seq_t)2 - (seq_t)cur;
            idx_t m = (idx_t)m_seq;

            /* Compute a(cur) = sum of previous terms – a(m) */
            seq_t val = sum - table[m - 1];
            table[num] = val;
            sum += val;
            num++;
        }
    }

    return table[n - 1];
}
