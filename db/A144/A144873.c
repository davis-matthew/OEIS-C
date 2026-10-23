/*
 * A144873  If n mod 8 = 0 or n mod 10 = 0 then floor(n^2/4)+n otherwise
 *          floor(n^2/4)+n-1.
 * https://oeis.org/A144873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2/4)+n if n mod 8 = 0 or n mod 10 = 0; otherwise
 *            a(n) = floor(n^2/4)+n-1. f(n) for all n >= 0
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;          /* convert index to seq_t for safety */
    seq_t sq = ns * ns;           /* n^2, may be large */
    seq_t base = sq / 4;          /* floor(n^2/4) */
    seq_t res = base + ns;        /* base + n */
    if ((n % 8) != 0 && (n % 10) != 0)
        res -= 1;
    return res;
}
