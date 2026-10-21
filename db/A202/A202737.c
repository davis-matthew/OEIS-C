/*
 * A202737  Number of nX3 0..1 arrays with row sums equal and column sums
 *          unequal to adjacent columns.
 * https://oeis.org/A202737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (3^n - 2 * Σ_{k=0}^{⌊n/2⌋} C(n,k) * C(n-k,k) + (n mod
 *            3 == 0 ? C(n,n/3) * C(2n/3,n/3) : 0))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Computes the number of n×3 0/1 arrays with equal row sums and unequal
   adjacent column sums, as defined in OEIS A202737. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* 3^n */
    seq_t total = 1;
    for (idx_t i = 0; i < n; ++i)
        total *= 3;

    /* Sum of C(n,k)*C(n-k,k) for k = 0..floor(n/2) */
    idx_t half = n / 2;
    seq_t term = 1;          /* term for current k */
    seq_t sum  = 0;          /* accumulating sum */
    seq_t t3   = 0;          /* term when k = n/3, if applicable */

    for (idx_t k = 0; k <= half; ++k)
    {
        sum += term;
        if ((n % 3) == 0 && k == n / 3)
            t3 = term;

        if (k == half)
            break;          /* finished */

        /* Compute term for k+1:
           term_{k+1} = term_k * (n-2k) * (n-2k-1) / ((k+1)^2)
           The division is exact integer division. */
        seq_t num1 = (seq_t)(n - 2 * k);
        seq_t num2 = (seq_t)(n - 2 * k - 1);
        seq_t den  = (seq_t)(k + 1) * (seq_t)(k + 1);
        term = term * num1 * num2 / den;
    }

    /* S = total - 2*sum + t3 */
    seq_t subtract = sum * 2;
    seq_t S = total - subtract + t3;

    /* Final answer a(n) = 2 * S */
    return S * 2;
}
