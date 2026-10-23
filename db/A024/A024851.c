/*
 * A024851  Least m such that if r and s in {-F(2*h) + phi*F(2*h-1): h =
 *          1,2,...,n} satisfy r < s, then r < k/m < s for some integer k,
 *          where F = A000045 (Fibonacci numbers) and phi = (1+sqrt(5))/2
 *          (golden ratio).
 * https://oeis.org/A024851
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A024851
   Formula: a(n) = L_{2n-3} + 1
   where L_k is the k-th Lucas number (L_0 = 2, L_1 = 1, L_k = L_{k-1} + L_{k-2}) */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(2) is the first term; behavior for n < 2 is undefined, return 0 */
    if (n < 2) return (seq_t)0;

    /* index of required Lucas number */
    idx_t k = 2 * n - 3;

    /* Lucas(0) = 2, Lucas(1) = 1 */
    if (k == 0) return (seq_t)3;           /* 2 + 1 */
    if (k == 1) return (seq_t)2;           /* 1 + 1 */

    seq_t prev2 = (seq_t)2;                /* L_0 */
    seq_t prev1 = (seq_t)1;                /* L_1 */
    for (idx_t i = 2; i <= k; ++i)
    {
        seq_t cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1 + (seq_t)1;               /* L_k + 1 */
}
