/*
 * A169338  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 37 * 36^(n-1) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   seq_from_str, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_from_str("1");

    /* cached constants 36 and 37 (initialised once) */
    static seq_t c36, c37;
    static int init = 0;
    if (!init) {
        c36 = seq_from_str("36");
        c37 = seq_from_str("37");
        init = 1;
    }

    /* compute 36^(n-1) */
    seq_t pow = seq_pow(c36, n - 1);

    /* multiply by 37 */
    seq_t res = seq_mul(c37, pow);

    return res;
}
