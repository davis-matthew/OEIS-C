/*
 * A145909  First 6-fold decimation of A061039. First bisection of A144454.
 * https://oeis.org/A145909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = A061039(6*n+3). For k = 2*n + 1 (odd integer), let t =
 *            k*k - 1. If k mod 3 == 0: a(n) = t // because A061039(3k) =
 *            (k^2-1)/8, so a = 8*(k^2-1)/8 = k^2-1 else: let x = t / 24 (t is
 *            always divisible by 24 when k%3≠0) if x mod 3 == 0: a(n) = (8 *
 *            x) / 3 // A061039 = t/72 in this case else: a(...
 * Range:     u8 n <= 7, u16 n <= 128, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n * 2 + 1;         /* k = 2n + 1 (odd) */
    seq_t t = k * k - 1;                /* k^2 - 1 */

    if (k % 3 == 0)       /* k divisible by 3 */
    {
        return t;          /* a(n) = k^2 - 1 */
    }
    else
    {
        seq_t x = t / 24;  /* t is divisible by 24 in this case */
        if (x % 3 == 0)
        {
            return (seq_t)8 * x / 3;   /* a(n) = (8*x)/3 */
        }
        else
        {
            return (seq_t)8 * x;        /* a(n) = 8*x */
        }
    }
}
