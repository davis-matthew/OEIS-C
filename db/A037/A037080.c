/*
 * A037080  In ternary expansion of n, reading from right to left, successive
 *          runs of the digits occur in order ...,0,1,2,0,1,2,...
 * https://oeis.org/A037080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  A037080 – return the n-th non‑negative integer whose ternary
 *  expansion, scanned from least significant to most, has runs of
 *  identical digits in the cyclic order 0,1,2,0,1,2,….
 *
 *  The function brute‑forces by testing successive integers and
 *  stopping when the desired count is reached.  It works for any
 *  seq_t type (fixed‑size or arbitrary‑precision) that supports
 *  addition, subtraction, multiplication, division, modulo, and
 *  equality/inequality with the integer constants 0, 1, 2, 3.
 */

#include <stdbool.h>

static bool valid_seq_t(seq_t x)
{
    /* 0, 1, and 2 are always valid. */
    if (x == 0)
        return true;

    seq_t t = x;
    seq_t cur = t % 3;           /* digit of the current run */

    while (t != 0) {
        /* Skip all digits of the current run. */
        do {
            t /= 3;
        } while (t != 0 && (t % 3) == cur);

        if (t == 0)           /* no more runs */
            break;

        seq_t next = t % 3;                     /* digit of the next run */
        seq_t expected = (cur + 1) % 3;          /* should be next digit in the cycle */
        if (next != expected)
            return false;

        cur = next;           /* continue with the new run */
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t  count = 0;      /* how many valid numbers we have seen */
    seq_t  m = 0;          /* current candidate integer */

    for (;;) {
        if (valid_seq_t(m)) {
            if (count == n)
                return m;
            ++count;
        }
        ++m;
    }
}
