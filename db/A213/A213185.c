/*
 * A213185  For the Collatz (3x+1) iteration starting at n, number of integers
 *          k such that T(n,k) = T(n-1,k) + 1 where T(n,k) is the k-th number
 *          of the trajectory (n is not counted).
 * https://oeis.org/A213185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 6910, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A213185: the number of indices k where the
   k‑th term of the Collatz trajectory starting at n exceeds the k‑th term
   of the trajectory starting at n‑1 by exactly 1.  The trajectory
   considered is the one after the starting number (the starting value
   is not counted). */
#include <stdbool.h>

static inline seq_t next_collatz(seq_t v)
{
    return (v % 2 == 0) ? v / 2 : v * 3 + 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* a(1) and a(0) are defined as 0 */

    seq_t val_n   = next_collatz((seq_t)n);          /* first term after n */
    seq_t val_nm1 = next_collatz((seq_t)(n - 1));    /* first term after n-1 */
    seq_t count   = 0;

    for (;;) {
        if (val_n == (val_nm1 + 1)) ++count;
        if (val_n == 1 || val_nm1 == 1) break;       /* last terms reached */
        val_n   = next_collatz(val_n);
        val_nm1 = next_collatz(val_nm1);
    }
    return count;
}
