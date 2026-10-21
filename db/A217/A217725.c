/*
 * A217725  Numbers n such that 5n is a partition number.
 * https://oeis.org/A217725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer n such that 5*n is a partition number
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq_array.h"

static seq_t *part = NULL;          /* array of partition numbers p(k) */
static size_t part_cap = 0;
static size_t part_sz = 0;          /* number of computed partitions, 1 initially (p(0)) */

static seq_t *vals = NULL;          /* array of sequence values a(j) */
static size_t vals_cap = 0;
static size_t vals_cnt = 0;         /* number of values stored */

/* ensure the helper arrays are initialized once */
static void init(void)
{
    if (!part) {
        grow_seq_array(&part, &part_cap, 1);   /* allocate space for p(0) */
        part[0] = (seq_t)1;                    /* p(0) = 1 */
        part_sz = 1;
    }
    if (!vals) {
        grow_seq_array(&vals, &vals_cap, 0);   /* start with empty array */
        vals_cnt = 0;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* offset is 1; behaviour undefined below 1 */
    init();

    while (vals_cnt < (size_t)n) {        /* generate until we have at least n values */
        idx_t k = part_sz;                /* compute p(k) where k = current size */

        seq_t sum = (seq_t)0;
        for (idx_t i = 1;; ++i) {
            idx_t pent1 = (i * (3 * i - 1)) / 2;
            if (pent1 > k) break;
            seq_t term = part[k - pent1];
            if (i & 1U) sum += term;      /* sign positive when i is odd */
            else sum -= term;              /* sign negative when i is even */

            idx_t pent2 = (i * (3 * i + 1)) / 2;
            if (pent2 <= k) {
                term = part[k - pent2];
                if (i & 1U) sum += term;
                else sum -= term;
            }
        }

        /* store new partition */
        if (part_sz == part_cap)
            grow_seq_array(&part, &part_cap, part_sz + 1);
        part[part_sz] = sum;
        ++part_sz;

        /* if divisible by 5, store the corresponding a value */
        if ((sum % (seq_t)5) == (seq_t)0) {
            seq_t val = sum / (seq_t)5;
            if (vals_cnt == vals_cap)
                grow_seq_array(&vals, &vals_cap, vals_cnt + 1);
            vals[vals_cnt] = val;
            ++vals_cnt;
        }
    }

    return vals[n - 1];
}
