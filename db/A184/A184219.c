/*
 * A184219  a(n) = A184218(n)/A130703(n) unless A130703(n) = 0 in which case
 *          a(n) = 0.
 * https://oeis.org/A184219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the “level” of triangular numbers. For n ≥ 5, let T =
 *            n(n+1)/2 and diff = T – (n+1). Then a(n) is the largest divisor
 *            d of diff with diff/d > n+1, i.e. the greatest d < (n–2)/2 that
 *            divides diff. For n < 5, a(n) = 0
 * Range:     u8 n <= 23, u16 n <= 1111, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    /* triangular number and the difference part */
    seq_t nn   = (seq_t)n;
    seq_t diff = (nn * (nn + 1) / 2) - (nn + 1);

    /* upper bound for the level: ⌊(n-2)/2⌋, but strictly less
       if (n-2) is even we must subtract one more. */
    seq_t max_a = (idx_t)(n - 2) / 2;
    if ((((n - 2) & 1ULL) == 0) && (max_a > 0))
        --max_a;

    /* find the largest divisor of diff not exceeding max_a */
    seq_t result = 1;
    for (seq_t d = max_a; d >= 1; --d)
    {
        if (diff % d == 0)
        {
            result = d;
            break;
        }
    }
    return result;
}
