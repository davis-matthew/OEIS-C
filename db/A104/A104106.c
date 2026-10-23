/*
 * A104106  a(1) = 1; thereafter, if A(k) = sequence of first 2^k -1 terms,
 *          then A(k+1) = A(k),1,A(k) if a(k) = 0, and A(k+1) = A(k),0,A(k) if
 *          a(k) = 1.
 * https://oeis.org/A104106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A089242(n) mod 2 (computed via the recursive construction
 *            using powers of two)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* recursive helper – internal */
static seq_t a_rec(idx_t n)
{
    if (n == 1)
        return 1;

    /* highest power of two not exceeding n */
    idx_t k   = 63 - __builtin_clzll(n);   /* n > 0 */
    idx_t mid = ((idx_t)1 << k);           /* 2^k */

    if (n == mid)                          /* middle element */
        return (seq_t)(1 - a_rec(k));

    /* otherwise skip past the middle and keep the right half */
    return a_rec(n - mid);
}

/* external entry point required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined in OEIS, return 0 for safety */
        return 0;
    return a_rec(n);
}
