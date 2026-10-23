/*
 * A013584  Smallest m such that 0!+1!+...+(m-1)! is divisible by n, or 0 if
 *          no such m exists.
 * https://oeis.org/A013584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 329, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A013584: smallest m such that 0!+1!+...+(m-1)! ≡ 0 (mod n),
   or 0 if no such m exists. */

#include <stdbool.h>

/* seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0)  /* safety: offset starts at 1, but guard anyway */
        return 0;

    seq_t modN = n;            /* cast to seq_t once for speed */
    seq_t sum   = 0;           /* current sum modulo n */
    seq_t fact  = 1 % modN;    /* (m-1)! modulo n; 0! = 1 */

    for (idx_t m = 1; m <= n; ++m) {
        sum = (sum + fact) % modN;
        if (sum == 0)          /* found smallest m */
            return m;
        fact = (fact * (seq_t)m) % modN;   /* next factorial (m)! */
    }
    return 0;                  /* no m < n satisfied the condition */
}
