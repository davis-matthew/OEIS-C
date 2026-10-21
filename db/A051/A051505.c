/*
 * A051505  a(n) = min { 2^n, floor(100/sqrt(n)) }.
 * https://oeis.org/A051505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ 2^(n+1), floor( 100 / sqrt(n+1) ) }
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n ≤ 4 the power of two is ≤ 44, so the minimum is 2^(n+1) */
    if (n <= 4) {
        seq_t v = (seq_t)1;
        v <<= (n + 1);           /* shift 0..5 bits – safe for all seq_t sizes   */
        return v;
    }

    idx_t N = n + 1;              /* argument for the floor term                 */
    /* When N > 10000, 100 / sqrt(N) < 1, so the floor is 0 */
    if (N > 10000ULL)
        return (seq_t)0;

    /* Binary search for the largest k such that k^2 * N ≤ 10000   */
    idx_t low = 0, high = 100;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned long long prod = (unsigned long long)mid * mid * (unsigned long long)N;
        if (prod <= 10000ULL)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}
