/*
 * A018408  Divisors of 352.
 * https://oeis.org/A018408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 352, sorted in ascending order
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t divs[12];
    static int cnt = 0;
    static int init = 0;

    if (!init) {
        const uint64_t M = 352ULL;          /* number whose divisors are sought   */
        uint64_t i = 1ULL;
        while (i * i <= M) {                /* find divisors in pairs            */
            if (M % i == 0ULL) {
                divs[cnt++] = (seq_t)i;
                uint64_t other = M / i;
                if (other != i) {
                    divs[cnt++] = (seq_t)other;
                }
            }
            ++i;
        }

        /* sort the 12 divisors ascending – small array, plain bubble sort */
        for (int x = 0; x < cnt - 1; ++x) {
            for (int y = x + 1; y < cnt; ++y) {
                if (divs[y] < divs[x]) {
                    seq_t tmp = divs[x];
                    divs[x] = divs[y];
                    divs[y] = tmp;
                }
            }
        }
        init = 1;
    }

    if (n >= 1 && (n - 1) < (idx_t)cnt) {
        return divs[(size_t)(n - 1)];
    }
    return (seq_t)0;
}
