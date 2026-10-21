/*
 * A225976  Number of n X 2 binary arrays whose sum with another n X 2 binary
 *          array containing no more than two 1s has rows and columns in
 *          lexicographically nondecreasing order.
 * https://oeis.org/A225976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*C(n+4,5) - 9*C(n+3,5) + 18*C(n+2,5) - 5*C(n+1,5) * +
 *            2*C(n,5) + C(n-1,5) * where C(m,5) = m!/(5!(m-5)!) =
 *            m*(m-1)*(m-2)*(m-3)*(m-4)/120. * The formula holds for all
 *            integer n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 136, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: binomial coefficient C(m,5) for m ≥ 5, zero otherwise. */
static inline seq_t binom5(idx_t m)
{
    if (m < 5) return 0;
    __int128 prod = (__int128)m * (m - 1) * (m - 2) * (m - 3) * (m - 4);
    prod /= 120; /* 5! = 120 */
    return (seq_t)prod;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but return 0 */
    __int128 result = 0;

    /* Each term is a small integer multiple of a binomial coefficient. */
    result += 4 * ( __int128 )binom5(n + 4);
    result -= 9 * ( __int128 )binom5(n + 3);
    result += 18 * ( __int128 )binom5(n + 2);
    result -= 5 * ( __int128 )binom5(n + 1);
    result += 2 * ( __int128 )binom5(n);
    result += ( __int128 )binom5(n - 1);

    return (seq_t)result;
}
