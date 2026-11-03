/*
 * A134421  Partial sums of A134021.
 * https://oeis.org/A134421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) + Σ_{k=1..n} f(k) where f(0)=1 and for k>0 f(k) =
 *            the smallest integer m ≥ 1 such that 3^m > 2k – 1 (i.e.,
 *            f(k)=⌊log_3(2k–1)⌋+1). This is A134021(k)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A134021(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t m = 2 * n - 1;          /* 2n-1 fits in idx_t (≤ 2^64-1)   */
    seq_t pow = (seq_t)3;         /* 3^1   */
    seq_t mm = (seq_t)m;          /* truncate to seq_t for comparison */
    idx_t k = 1;                  /* current exponent    */

    /* Find smallest k such that 3^k > 2n-1 */
    while (pow <= mm)
    {
        pow *= (seq_t)3;
        ++k;
    }
    return (seq_t)k;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)1;         /* a(0) = 1 */

    if (n == 0)
        return sum;

    for (idx_t i = 1; i <= n; ++i)
    {
        sum += A134021(i);
    }
    return sum;
}
