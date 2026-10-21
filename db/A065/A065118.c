/*
 * A065118  Numbers which are 19 times the sum of their digits.
 * https://oeis.org/A065118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the nth positive integer such that a = 19 *
 *            digit_sum(a). The sequence is finite; the largest term is 399
 *            (a(10)), so all terms are < 1000. This implementation
 *            precomputes the entire list by scanning all multiples of 19 up
 *            to 1000 and storing those that satisfy the property
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t digit_sum_seq_t(seq_t value)
{
    seq_t sum = 0;
    while (value != 0) {
        sum += value % 10;
        value /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Static cache of all sequence values. */
    static seq_t cache[20];
    static idx_t counter = 0;

    if (counter == 0) {
        /* The maximum possible term is bounded by 19 * 27 = 513
           (since sum of digits <= 27 for any 3‑digit number),
           and numbers with 4 digits or more cannot satisfy the
           equation because 10^(d-1) > 171*d for d >= 4.
           Therefore we only need to check multiples of 19 up to 1000. */
        seq_t m = 19;               /* first multiple of 19 */
        const seq_t limit = 1000;   /* safe upper bound */

        while (m <= limit) {
            seq_t sum = digit_sum_seq_t(m);
            if (sum == m / 19) {     /* integer division is exact here */
                cache[counter++] = m;
            }
            m += 19;
        }
    }

    if (n < counter) {
        return cache[n];
    }
    return 0;   /* undefined for indices beyond the last term */
}
