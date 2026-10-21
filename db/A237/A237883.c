/*
 * A237883  Numbers k such that LR0(k) < LR1(k), where LR0(k) = A087117(k) is
 *          the length of the longest run of zeros in the binary
 *          representation of k, LR1(k) = A038374(k) is the length of the
 *          longest run of ones.
 * https://oeis.org/A237883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that LR0(k) < LR1(k)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   lr0_lr1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lr0_lr1.h"
/* LR0(k) = longest run of zeros in binary representation of k  */
/* LR1(k) = longest run of ones  in binary representation of k */

/* Returns the n-th term (1‑based) of OEIS sequence A237883. */
seq_t a(idx_t n)
{
    idx_t count = 0ULL;
    idx_t k     = 1ULL;
    idx_t l0, l1;

    while (1) {
        lr0_lr1(k, &l0, &l1);
        if (l0 < l1) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
