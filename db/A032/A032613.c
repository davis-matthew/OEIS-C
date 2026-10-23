/*
 * A032613  Concatenation of n and n + 8 or {n,n+8}.
 * https://oeis.org/A032613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*10^digits(n+8) + (n+8)
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* Compute n+8 as idx_t (no risk of overflow for idx_t type) */
    idx_t n_plus = n + (idx_t)8;

    /* Number of decimal digits of n+8 */
    int d = digits(n_plus);

    /* 10^digits(n+8) as seq_t */
    seq_t shift = pow10((idx_t)d);

    /* Concatenate n and n+8 */
    seq_t n_seq = (seq_t)n;
    seq_t n_plus_seq = (seq_t)n_plus;
    seq_t res = n_seq * shift + n_plus_seq;

    return res;
}
