/*
 * A214301  Smallest limiting value of n under iteration of "Sum of its digits
 *          raised to its digits power" (A045512).
 * https://oeis.org/A214301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest element reached by iterating f(x) =
 *            Σ_{d∈digits(x)} d^d (0^0 defined as 0)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const seq_t POWD[10] = {
    (seq_t)0,                 /* 0^0 = 0 */
    (seq_t)1,                 /* 1^1 */
    (seq_t)4,                 /* 2^2 */
    (seq_t)27,                /* 3^3 */
    (seq_t)256,               /* 4^4 */
    (seq_t)3125,              /* 5^5 */
    (seq_t)46656,             /* 6^6 */
    (seq_t)823543,            /* 7^7 */
    (seq_t)16777216,          /* 8^8 */
    (seq_t)387420489          /* 9^9 */
};

static seq_t f(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        sum += POWD[d];
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;
    /* immediate fixed point */
    if (f(start) == start) {
        return start;
    }

    /* Floyd’s cycle‑finding algorithm */
    seq_t tortoise = f(start);
    seq_t hare = f(f(start));
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(f(hare));
    }

    /* Find cycle entry point */
    tortoise = start;
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(hare);
    }

    /* Find minimal element in the cycle */
    seq_t cur = tortoise;
    seq_t min = cur;
    do {
        cur = f(cur);
        if (cur < min) {
            min = cur;
        }
    } while (cur != tortoise);

    return min;
}
