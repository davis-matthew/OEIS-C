/*
 * A173345  Number of trailing zeros of the superfactorial of n (A000178).
 * https://oeis.org/A173345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = Sum_{k=1..n} Sum_{e≥1} ⌊k / 5^e⌋, i.e. the total number
 *            of trailing zeros of the superfactorial of n
 * Range:     u8 n <= 49, u16 n <= 731, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t ans = 0;
    seq_t N   = (seq_t)n;

    /* t = ⌊N / d⌋ for each power d = 5^e. */
    for (seq_t d = (seq_t)5; d <= N; d *= (seq_t)5)
    {
        seq_t t = N / d;
        /* sum_{k=1..N} ⌊k/d⌋ = (N+1)*t - d*t*(t+1)/2 */
        ans += (N + (seq_t)1) * t - d * t * (t + (seq_t)1) / (seq_t)2;
    }

    return ans;
}
