/*
 * A075877  Powering the decimal digits of n (left-associative).
 * https://oeis.org/A075877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hear, look
 * Formula:   a(n) = a(floor(n/10))^(n mod 10), a(n)=n for n<10
 * Range:     u8 n <= 27, u16 n <= 47, u32 n <= 247, u64 n <= 287,
 *            u128 n <= 398, big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper _pow_seq_t defined in the utilities. */
#include "../utils/_pow_seq_t.h"

seq_t a(idx_t n)
{
    if (n < 10u)         /* base cases a(0)...a(9) */
        return (seq_t)n;

    /* recursive computation: left‑associate the decimal digits */
    seq_t base = a(n / 10u);
    idx_t  exp  = (idx_t)(n % 10u);
    return _pow_seq_t(base, exp);
}
