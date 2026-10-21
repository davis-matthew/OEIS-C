/*
 * A063245  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          98 ).
 * https://oeis.org/A063245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 + ⌊(n-1)/6⌋·41 + P[(n-1) mod 6] where P = {0, 7, 15,
 *            20, 28, 35}. This reproduces the OEIS sequence A063245
 *            (dimension of weight-2n cuspidal newforms for Γ₀(98)). The
 *            pattern repeats every 6 indices because the underlying dimension
 *            formula contains floor terms with denominators 2,3,4, gi...
 * Range:     u8 n <= 37, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t prefix[6] = {0, 7, 15, 20, 28, 35};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* off‑offset values are irrelevant */

    idx_t k = n - 1;
    idx_t blocks = k / 6;          /* number of complete 6‑blocks */
    idx_t rem   = k % 6;           /* position inside the current block */

    /* 3 + blocks*41 + prefix[rem]  (all in seq_t arithmetic) */
    seq_t result = 3;
    result += (seq_t)blocks * 41;
    result += prefix[rem];

    return result;
}
