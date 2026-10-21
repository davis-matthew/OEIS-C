/*
 * A051626  Period of decimal representation of 1/n, or 0 if 1/n terminates.
 * https://oeis.org/A051626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: computes period of decimal representation of 1/n
 * The period (or length of the repeating block) is 0 if 1/n terminates.
 * For n > 0, let m = n after removing all factors 2 and 5.
 * If m == 1, the decimal terminates, answer 0.
 * Otherwise the period is the multiplicative order of 10 modulo m,
 * i.e. the smallest k > 0 such that 10^k ≡ 1 (mod m).
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but safe */

    /* remove all factors 2 and 5 */
    idx_t m = n;
    while ((m & 1ULL) == 0) m >>= 1;          /* divide by 2 */
    while (m % 5ULL == 0) m /= 5ULL;          /* divide by 5 */

    if (m == 1) return 0;

    /* multiplicative order of 10 modulo m */
    idx_t k = 1;
    idx_t t = 10 % m;                         /* 10^1 mod m */
    while (t != 1) {
        t = (t * 10) % m;
        ++k;
    }
    return (seq_t)k;
}
