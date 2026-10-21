/*
 * A077379  Smallest n-digit squarefree number whose internal as well as
 *          external digits form a squarefree number greater than 1; or 0 if
 *          no such number exists.
 * https://oeis.org/A077379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest n‑digit squarefree number whose internal
 *            digits and external digits are also squarefree. For n < 3 there
 *            is no such number, a(n) = 0. For n ≥ 4 the pattern 10^(n‑1) + 21
 *            works, except when n ≡ 3 (mod 22) in which case 10^(n‑1) + 23 is
 *            needed. This follows the OEIS description...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"   /* seq_t pow10(idx_t e); */

seq_t a(idx_t n)
{
    if (n < 3)                 /* a(1) = 0, a(2) = 0 */
        return (seq_t)0;

    seq_t base = pow10(n - 1);                 /* 10^(n-1)  */
    seq_t add  = (n % 22 == 3ULL) ? (seq_t)23 : (seq_t)21;
    return base + add;
}
