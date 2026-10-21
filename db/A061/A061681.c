/*
 * A061681  a(0)=1; a(n) = a(n-1) + lead(a(n-1)) for n > 0 where for an
 *          integer x lead(x) is the leading digit in base 10.
 * https://oeis.org/A061681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, nice, look
 * Formula:   a(0) = 1; a(n) = a(n-1) + lead(a(n-1)) for n > 0, where lead(x)
 *            is the leading decimal digit of x. The implementation iterates
 *            from 0 to n, producing each term directly. It runs in O(n·d)
 *            time, where d is the number of decimal digits of the current
 *            value (at most 39 for a 128‑bit type). This simp...
 * Range:     u8 n <= 151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t leading_digit(seq_t v)
{
    /* Return the first decimal digit of v.  Works for any unsigned
       seq_t type; the loop runs at most 39 times for 128‑bit numbers. */
    while (v >= 10)
        v /= 10;
    return v;
}

seq_t a(idx_t n)
{
    seq_t val = 1;          /* a(0) */
    if (n == 0)
        return val;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t d = leading_digit(val);
        val += d;
    }
    return val;
}
