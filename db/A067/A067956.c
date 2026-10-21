/*
 * A067956  Number of nodes in virtual, "optimal", chordal graphs of diameter
 *          4 and degree n+1.
 * https://oeis.org/A067956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*t*(t+1)*(t^2+t+4))/3)+1, if n is odd, t = (n+1)/2
 *            ((2*t*(t+1)*(t^2+t+4))/3)+1 +((2*t+1)*(2*t^2+2*t+3))/3, if n is
 *            even, t = n/2 (All divisions are integer divisions.)
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 564, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1 – undefined for n==0, but return 0 */
    unsigned long long t = (n & 1ULL) ? (n + 1ULL) / 2ULL : n / 2ULL;

    /* part1 = ((2*t*(t+1)*(t^2+t+4))/3)+1  */
    unsigned long long prod1 = 2ULL * t * (t + 1ULL) * (t * t + t + 4ULL);
    unsigned long long part1 = prod1 / 3ULL + 1ULL;

    unsigned long long result = part1;

    if ((n & 1ULL) == 0ULL) { /* n even: add the second term */
        unsigned long long prod2 = (2ULL * t + 1ULL) * (2ULL * t * t + 2ULL * t + 3ULL);
        unsigned long long part2 = prod2 / 3ULL;
        result += part2;
    }

    return (seq_t)result;
}
