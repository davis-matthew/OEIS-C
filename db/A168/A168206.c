/*
 * A168206  a(n) = 5*n - a(n-1) + 1 with n>1, a(1)=0.
 * https://oeis.org/A168206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7 + 17*(-1)^n)/4. Recurrence: a(1)=0; a(n)=5*n -
 *            a(n-1) + 1 for n>1
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long nn = n;
    unsigned long long numerator = 10ULL * nn + 7ULL;

    if ((n & 1ULL) == 0)      /* n even: add 17 */
        numerator += 17ULL;
    else                      /* n odd: subtract 17 (safe: numerator >=17) */
        numerator -= 17ULL;

    return (seq_t)(numerator / 4ULL);
}
