/*
 * A136758  a(n) = leading digit of n! in base 7.
 * https://oeis.org/A136758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the leading digit of n! when written in base 7
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n≥0: a(0)=1, a(1)=1. For n>1 we compute the fractional part of log₇(n!)
   and use it to obtain the leading digit: a(n)=⌊7^frac⌋ where
   frac = sum_{k=2}^{n} log₇(k) - ⌊…⌋ . */

#include <math.h>

seq_t a(idx_t n)
{
    /* 0! and 1! are 1, whose leading digit in any base is 1 */
    if (n <= 1)
        return (seq_t)1;

    /* one‑time constant: 1 / log(7) in long double */
    static const long double inv_log7 = 1.0L / logl(7.0L);

    /* Sum of log₇(k) for k=2..n */
    long double sum = 0.0L;
    for (idx_t k = 2; k <= n; ++k)
        sum += logl((long double)k) * inv_log7;

    /* Separate integer and fractional parts of the sum */
    long double int_part;
    long double frac = modfl(sum, &int_part);

    /* Leading digit = floor(7^frac).  Add a tiny epsilon to guard
       against round‑off errors that might push a very close value
       just below an integer. */
    long double pow_val = powl(7.0L, frac);
    unsigned int digit = (unsigned int)floorl(pow_val + 1e-12L);

    /* Clamp to a valid base‑7 digit range (1..6) */
    if (digit < 1) digit = 1;
    if (digit > 6) digit = 6;

    return (seq_t)digit;
}
