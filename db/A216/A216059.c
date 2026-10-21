/*
 * A216059  Smallest number not in Collatz trajectory starting at n.
 * https://oeis.org/A216059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer not appearing in the Collatz
 *            trajectory starting at n
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    unsigned char seen[8] = {0};

    while (1) {
        if (cur >= (seq_t)2 && cur <= (seq_t)7)
            seen[(int)cur] = 1;
        if (cur == (seq_t)1)
            break;
        if ((cur & (seq_t)1) == (seq_t)0)
            cur = cur / (seq_t)2;
        else
            cur = cur * (seq_t)3 + (seq_t)1;
    }

    for (int k = 2; k <= 7; ++k) {
        if (!seen[k])
            return (seq_t)k;
    }
    /* The sequence guarantees an answer <= 7, so this point should never be reached. */
    return (seq_t)8;
}
