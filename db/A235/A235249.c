/*
 * A235249  Fixed point when A001175 (Pisano periods) is iterated with
 *          starting value n.
 * https://oeis.org/A235249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of iterating Pisano period A001175 */
#include <stdint.h>

/* Compute the Pisano period of modulus m.
   The period is the smallest positive k such that
   Fibonacci mod m repeats the pair (0,1). */
static seq_t pisano_period(seq_t m)
{
    if (m == 1) return 1;               /* modulo 1 has period 1 */
    seq_t prev = 0, cur = 1;
    idx_t k = 0;
    do {
        seq_t next = prev + cur;        /* sum < 2*m */
        if (next >= m) next -= m;       /* modulo m */
        prev = cur;
        cur = next;
        ++k;
    } while (!(prev == 0 && cur == 1));
    return (seq_t)k;
}

/* Sequence a(n).  For n>=1 returns the fixed point of iterating
   the Pisano period starting from n. */
seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t cur = (seq_t)n;
    while (1) {
        seq_t next = pisano_period(cur);
        if (next == cur) return cur;
        cur = next;
    }
}
