/*
 * A220182  Number of changes of parity in the Collatz trajectory of n.
 * https://oeis.org/A220182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of parity changes in the Collatz trajectory of n
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;   /* a(1) = 0 */

    seq_t cur = (seq_t)n;
    seq_t cnt = 0;
    bool prev_parity = (bool)(cur & 1);

    while (cur != 1) {
        if (cur & 1)
            cur = cur * 3 + 1;
        else
            cur = cur / 2;

        bool cur_parity = (bool)(cur & 1);
        if (cur_parity != prev_parity)
            ++cnt;
        prev_parity = cur_parity;
    }
    return cnt;
}
