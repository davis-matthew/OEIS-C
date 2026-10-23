/*
 * A011771  Days per century for Roman calendar from first century, following
 *          Gregorian calendar after A.D. 1582.
 * https://oeis.org/A011771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36500 + L(n) where L(n) is the number of leap years in
 *            the nth century. • For centuries 1..15 (Julian calendar) → 25
 *            leaps → 36525 days. • Century 16: Julian rule, but 10 days
 *            omitted in Oct 1582 ⇒ 36515 days. • For centuries ≥17 (Gregorian
 *            calendar) → count years divisible by 4, subtract...
 * Range:     u8 none, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 15ULL)
        return 36525;
    if (n == 16ULL)
        return 36515;

    seq_t start = (seq_t)(n - 1) * 100 + 1;
    seq_t end   = (seq_t)n * 100;
    seq_t c4    = end / 4 - (start - 1) / 4;
    seq_t c100  = end / 100 - (start - 1) / 100;
    seq_t c400  = end / 400 - (start - 1) / 400;
    seq_t leaps = c4 - c100 + c400;
    return 36500 + leaps;
}
