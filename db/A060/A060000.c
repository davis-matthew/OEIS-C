/*
 * A060000  If the numbers a(1)...a(n) contain a hole, then a(n+1) is the
 *          smallest hole; otherwise a(n+1) = a(n-1) + a(n).
 * https://oeis.org/A060000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = n-1 for n < 7 or n ≠ 3·2^m+4 (m ≥ 1); * a(n) = 2 n − 5
 *            for n = 3·2^m+4 with m ≥ 1
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* first six terms given explicitly */
    if (n <= 6) {
        switch ((uint64_t)n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)3;
            case 4: return (seq_t)5;
            case 5: return (seq_t)4;
            case 6: return (seq_t)9;
        }
    }

    /* For n ≥ 7, check whether n = 3·2^m + 4 with m ≥ 1.
     * 1. n ≥ 10 guarantees m ≥ 1 (since 3·2^0+4 = 7).
     * 2. (n−4) must be divisible by 3.
     * 3. (n−4)/3 must be a power of two. */
    if (n >= 10) {
        idx_t div = n - 4;
        if (div % 3 == 0) {
            idx_t m = div / 3;
            /* power-of-two test: m > 0 and m & (m-1) == 0 */
            if (m && (m & (m - 1)) == 0) {
                return (seq_t)n * (seq_t)2 - (seq_t)5;
            }
        }
    }

    /* Default case */
    return (seq_t)n - (seq_t)1;
}
