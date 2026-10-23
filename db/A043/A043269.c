/*
 * A043269  a(n) is the sum of the digits of n-th base-10 palindrome.
 * https://oeis.org/A043269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum of digits of the n-th base-10 palindrome (OEIS
 *            A043269)
 * Range:     u8 n <= 35, u16 n <= 756, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    /* Handle the first 10 palindromes: 0-9 */
    if (n <= 10) {
        return (seq_t)(n - 1);          /* sum of digits of n-1 is n-1 itself */
    }

    /* Offset after the first 10 palindromes */
    idx_t m = n - 10;
    idx_t len = 2;                     /* start with 2-digit palindromes */

    /* Find the length of the n-th palindrome */
    while (1) {
        idx_t half_len = (len + 1) / 2;               /* ceil(len/2) */
        /* count of palindromes with this length: 9 * 10^(half_len-1) */
        idx_t count = 9;
        for (idx_t i = 1; i < half_len; ++i) {
            count *= 10;
        }
        if (m <= count)
            break;
        m -= count;
        ++len;
    }

    /* Generate the specific palindrome */
    idx_t half_len = (len + 1) / 2;
    unsigned long long half_start = 1ULL;
    for (idx_t i = 1; i < half_len; ++i)
        half_start *= 10ULL;

    unsigned long long half = half_start + (m - 1);
    unsigned long long pal = half;
    unsigned long long temp = half;

    if (len % 2 == 1)      /* skip middle digit for odd length */
        temp /= 10;

    while (temp > 0) {
        pal = pal * 10 + (temp % 10);
        temp /= 10;
    }

    return digit_sum((seq_t)pal);
}
