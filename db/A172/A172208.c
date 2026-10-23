/*
 * A172208  Number of ways to place 4 nonattacking bishops on a 4 X n board.
 * https://oeis.org/A172208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (32*n^4 -336*n^3 +1702*n^2 -4701*n +5844) / 3 (n>=9)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 144, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For 1 <= n <= 8 use the given initial values. */

#include <stdint.h>   /* for uint64_t */

seq_t
a(idx_t n)
{
    /* initial terms for n = 1..8 */
    static const seq_t init[8] = {
        1,     /* a(1) */
        9,     /* a(2) */
        61,    /* a(3) */
        260,   /* a(4) */
        927,   /* a(5) */
        2578,  /* a(6) */
        5965,  /* a(7) */
        12066  /* a(8) */
    };

    if (n <= 8 && n >= 1) {
        return init[n - 1];
    }

    /* use 64‑bit arithmetic for the intermediate calculation;
       the formula is valid and yields an integer result for all n >= 9 */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long n2  = nn * nn;            /* n^2   */
    unsigned long long n3  = n2 * nn;            /* n^3   */
    unsigned long long n4  = n3 * nn;            /* n^4   */

    unsigned long long numerator =
        32ULL * n4
      - 336ULL * n3
      + 1702ULL * n2
      - 4701ULL * nn
      + 5844ULL;

    /* division by 3 is exact for all n >= 9 */
    seq_t result = (seq_t)(numerator / 3ULL);
    return result;
}
