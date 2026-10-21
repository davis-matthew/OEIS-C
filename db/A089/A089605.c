/*
 * A089605  Let P(m) = m/2 if m is even, m + rev(m) if m is odd, where rev(m)
 *          is m's base 10 representation reversed. It is conjectured that any
 *          number k eventually cycles when P is repeatedly applied to it.
 *          Sequence gives number of steps before the cycle is reached.
 * https://oeis.org/A089605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of steps to reach a cycle when applying P
 *            repeatedly to n, where P(m) = m/2 if m is even m + rev(m) if m
 *            is odd (rev is the decimal digit reversal of m)
 * Range:     u8 n <= 48, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility for digit reversal – part of the common utils library */
#include "../utils/reverse_digits.h"

/* Next value under the transformation P */
static inline seq_t nextP(seq_t x)
{
    if ((x & (seq_t)1) == 0)            /* even */
        return x / (seq_t)2;
    return x + reverse_digits(x);      /* odd  */
}

/* Number of steps before the first repetition (tail length) */
seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;

    /* --- Floyd's cycle‑finding algorithm --- */
    seq_t tortoise = nextP(start);
    seq_t hare     = nextP(nextP(start));

    while (tortoise != hare) {
        tortoise = nextP(tortoise);
        hare     = nextP(nextP(hare));
    }

    /* --- Find the start of the cycle (μ) --- */
    size_t mu = 0;
    tortoise = start;
    while (tortoise != hare) {
        tortoise = nextP(tortoise);
        hare     = nextP(hare);
        mu++;
    }

    /* μ fits in seq_t because the sequence terminates for all n ≤ 10000 */
    return (seq_t)mu;
}
