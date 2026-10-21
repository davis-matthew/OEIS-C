/*
 * A220071  Difference between number of halving steps and number of tripling
 *          steps needed to reach 1 in '3x+1' problem.
 * https://oeis.org/A220071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (#halving steps) – (#tripling steps) in the Collatz
 *            (3x+1) process
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)0;
    }

    long long diff = 0;                 /* signed counter for halving minus tripling */
    unsigned long long cur = (unsigned long long)n;

    while (cur != 1ULL) {
        if ((cur & 1ULL) == 0ULL) {     /* even -> halving step */
            cur >>= 1;
            diff += 1;
        } else {                       /* odd -> tripling step */
            cur = cur * 3ULL + 1ULL;
            diff -= 1;
        }
    }

    return (seq_t)diff;                /* final difference is non‑negative */
}
