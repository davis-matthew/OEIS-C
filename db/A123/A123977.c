/*
 * A123977  Complement of A061909 (skinny numbers).
 * https://oeis.org/A123977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complement of A061909 (skinny numbers) – enumerative algorithm. */

#include <stdbool.h>

static bool qualifies(seq_t x)
{
    /* For single‑digit numbers only 4–9 are accepted. */
    if (x < (seq_t)10)
        return x >= (seq_t)4;

    /* Find the most significant digit. */
    seq_t first = x;
    while (first >= (seq_t)10)
        first /= (seq_t)10;

    int first_digit = (int)first;           /* 1–9 */
    int threshold;

    /* For leading digits 4 or greater, no numbers are skinny, so all are accepted. */
    if (first_digit >= 4)
        threshold = 0;
    else                       /* first_digit 1,2,3 */
        threshold = 5 - first_digit;            /* 4,3,2 respectively */

    seq_t last = x % (seq_t)10;
    return last >= (seq_t)threshold;
}

seq_t a(idx_t n)
{
    idx_t  count = 0;
    seq_t  num   = (seq_t)4;          /* the first possible term */

    while (1) {
        if (qualifies(num)) {
            ++count;
            if (count == n)
                return num;
        }
        ++num;
    }
}
