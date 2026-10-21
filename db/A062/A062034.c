/*
 * A062034  Positive numbers whose product of digits is twice the sum of the
 *          digits.
 * https://oeis.org/A062034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 101, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   alloc_seq_array, digit_product, digit_sum, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code implements sequence A062034: numbers whose digit product equals twice their digit sum */
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next = 1;               /* integer to test */

    if (n == 0)
        return (seq_t)0;                 /* offset starts at 1; undefined for n=0 */

    while (seq_len < n)
    {
        seq_t x = (seq_t)next;
        seq_t sum = digit_sum(x);
        seq_t prod = digit_product(x);
        if (prod == 2 * sum)
        {
            if (seq_len == seq_cap)                /* grow cache if full */
            {
                size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                seq_t *tmp = alloc_seq_array(new_cap);
                for (size_t i = 0; i < seq_len; ++i)
                    tmp[i] = seq[i];
                free_seq_array(seq);
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = x;
        }
        ++next;
    }
    return seq[n - 1];
}
