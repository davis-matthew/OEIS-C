/*
 * A018510  Divisors of 544.
 * https://oeis.org/A018510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 544 in ascending order (0 if n>12)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 544 = 2^5 * 17, so it has (5+1)*(1+1)=12 divisors */
    /* Generate all divisors, sort, and return the (n-1)-th element. */

    /* There are always 12 divisors for 544. */
    const size_t DIV_CNT = 12;
    seq_t divisors[DIV_CNT];
    size_t count = 0;

    for (unsigned e1 = 0; e1 <= 5; ++e1) {
        /* 2^e1 can be computed safely by shift */
        seq_t val1 = ((seq_t)1) << e1;
        for (unsigned e2 = 0; e2 <= 1; ++e2) {
            seq_t val2 = (e2 == 1) ? (seq_t)17 : (seq_t)1;
            divisors[count++] = val1 * val2;
        }
    }

    /* Simple insertion sort (12 elements, trivial time). */
    for (size_t i = 0; i < count; ++i) {
        for (size_t j = i + 1; j < count; ++j) {
            if (divisors[j] < divisors[i]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }

    if (n == 0 || n > DIV_CNT)
        return 0;                      /* out of range */
    return divisors[n - 1];
}
