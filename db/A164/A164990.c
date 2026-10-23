/*
 * A164990  Number of square involutions of n.
 * https://oeis.org/A164990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   binom_n_k, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n+2)*2^(n-3) - (n-2)*C(n-3,(n-3)/2), n > 1
 *          a(1) = 1
 *          a(2) = 2
 */
#include "../utils/pow_uint.h"
#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n < 1) return 0;          /* safeguard: undefined for n < 1 */

    /* n >= 3 */
    seq_t power = pow_uint(2, n - 3);           /* 2^(n-3)  */
    seq_t term1 = (seq_t)(n + 2) * power;       /* (n+2)*2^(n-3) */
    idx_t k     = (n - 3) / 2;                  /* floor((n-3)/2) */
    seq_t comb  = binom_n_k(n - 3, k);          /* C(n-3, k)  */
    seq_t term2 = (seq_t)(n - 2) * comb;        /* (n-2)*C(...) */

    return term1 - term2;
}
