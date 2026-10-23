/*
 * A034311  Decimal part of a(n)^(1/5) starts with reversal of its integer
 *          part: first term of runs.
 * https://oeis.org/A034311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   digits, pow10, pow_seq, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For k = n + 1, let d = number of decimal digits of k,
   T = k * 10^d + rev(k).
   Then a(n) = ceil( T^5 / 10^(5d) ).  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/pow_seq.h"
#include "../utils/rev.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* k = n + 1 */
    idx_t k = n + 1;

    /* d = number of digits of k */
    unsigned int d = digits(k);

    /* 10^d and 10^(5d) as seq_t */
    seq_t ten_d = pow10((idx_t)d);
    seq_t ten_5d = pow10((idx_t)(5 * d));

    /* rev(k) as seq_t */
    seq_t rev_k = (seq_t)rev(k);

    /* T = k * 10^d + rev(k) */
    seq_t T = ((seq_t)k) * ten_d + rev_k;

    /* T^5 */
    seq_t T5 = pow_seq(T, 5);

    /* a(n) = ceil(T5 / 10^(5d)) = (T5 + 10^(5d) - 1) / 10^(5d) */
    seq_t L = (T5 + ten_5d - (seq_t)1) / ten_5d;

    return L;
}
