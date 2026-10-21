/*
 * A081170  Triangle in which n-th row gives trajectory of n (including n
 *          itself) under the map x -> x/2 if x is even, x -> x-3 if x is odd,
 *          stopping when reach 0 or 1.
 * https://oeis.org/A081170
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A081170: flattened triangle of trajectories under map
   x -> x/2 if even, x -> x-3 if odd */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* the sequence is defined only for n ≥ 3 (offset 3) */
    if (n < 3)
        return (seq_t)0;

    /* zero‑based position in the flattened triangle */
    idx_t pos = n - 3;

    /* find the row r that contains this position */
    idx_t r = 3;
    while (1) {
        /* compute the length of the trajectory starting at r */
        seq_t x = (seq_t)r;
        size_t len = 0;
        do {
            ++len;
            if (x == (seq_t)0 || x == (seq_t)1)
                break;
            x = (x & (seq_t)1) ? x - (seq_t)3 : x >> 1;
        } while (1);

        if (pos < (idx_t)len)
            break;                 /* r is the desired row */

        pos -= (idx_t)len;
        ++r;
    }

    /* compute the element at index 'pos' within row r */
    seq_t val = (seq_t)r;
    while (pos-- > 0) {
        if (val == (seq_t)0 || val == (seq_t)1)
            break;
        val = (val & (seq_t)1) ? val - (seq_t)3 : val >> 1;
    }
    return val;
}
