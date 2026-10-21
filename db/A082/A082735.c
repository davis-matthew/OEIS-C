/*
 * A082735  Product of n-th group of terms in A074147.
 * https://oeis.org/A082735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 1 * For n odd (n = 2k-1): a(n) = ∏_{i=0}^{2k-2} (
 *            2·(k-1)² + 1 + 2i ) * For n even (n = 2k): a(n) = ∏_{i=0}^{2k-1}
 *            ( 2·((k-1)·k + 1) + 2i ) * * The product uses the next
 *            consecutive odd or even numbers, starting * at the formulas
 *            shown. No state is kept between calls, so any * order of c...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined for n≥1 */
        return (seq_t)0;

    if (n & 1) {         /* odd n:  n = 2k-1 */
        idx_t k = (n + 1) >> 1;                 /* k = (n+1)/2          */
        idx_t size = (k << 1) - 1;              /* size = 2k-1          */
        idx_t start = (idx_t)(2) * ((k - 1) * (k - 1)) + 1;  /* 2*(k-1)^2 + 1 */

        seq_t prod = 1;
        for (idx_t i = 0; i < size; ++i) {
            seq_t term = (seq_t)start + (seq_t)(2 * i);
            prod *= term;
        }
        return prod;
    } else {             /* even n:  n = 2k */
        idx_t k = n >> 1;                         /* k = n/2              */
        idx_t size = k << 1;                      /* size = 2k             */
        idx_t start = (idx_t)(2) * (((k - 1) * k) + 1);  /* 2*((k-1)k+1) */

        seq_t prod = 1;
        for (idx_t i = 0; i < size; ++i) {
            seq_t term = (seq_t)start + (seq_t)(2 * i);
            prod *= term;
        }
        return prod;
    }
}
