/*
 * A125710  In the "3x+1" problem, let 0 denote a halving step and 1 denote an
 *          x->3x+1 step. Then a(n) is obtained by writing the sequence of
 *          steps needed to reach 1 from 2n+1 and reading it as a decimal
 *          number.
 * https://oeis.org/A125710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary number formed by the Collatz steps (0 = halving, 1
 *            = 3x+1) starting from 2n+1 and stopping when the value 1 is
 *            reached again; that bitstring is interpreted as a decimal number
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 12, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit type for the Collatz value to avoid overflow
       for the given limits. */
    unsigned __int128 m = (unsigned __int128)(2 * (unsigned __int128)n + 1);
    seq_t res = 0;

    while (1) {
        unsigned step = (unsigned)(m & 1);      /* 1 if odd, 0 if even */
        m = step ? m * 3 + 1 : m / 2;           /* Collatz step */
        res = (res << 1) | (seq_t)step;         /* build bitstring */
        if (m == 1) break;                      /* stop when 1 appears again */
    }
    return res;
}
