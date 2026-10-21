/*
 * A068190  Largest number whose digit product equals n; a(n)=0 if no such
 *          number exists, e.g., when n has a prime factor larger than 7; no
 *          digit=1 is permitted to avoid an infinite number of solutions.
 * https://oeis.org/A068190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 0 if n has a prime factor > 7; otherwise a(n) is the
 *            number obtained by concatenating the prime factors {7,5,3,2} of
 *            n in descending order (each factor repeated as many times as it
 *            appears in the factorisation of n). Example: n = 18 = 2·3² ->
 *            a(18) = 3·3·2 = 332
 * Range:     u8 n <= 11, u16 n <= 63, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* safety for an undefined 0‑index */
        return 0;

    uint64_t rem = n;

    /* Count prime factors 2,3,5,7 */
    uint64_t cnt[4] = {0, 0, 0, 0};  // cnt[0]=2, cnt[1]=3, cnt[2]=5, cnt[3]=7
    while ((rem & 1ULL) == 0) {      // factor 2
        cnt[0]++;
        rem >>= 1;
    }
    while (rem % 3ULL == 0) {
        cnt[1]++;
        rem /= 3ULL;
    }
    while (rem % 5ULL == 0) {
        cnt[2]++;
        rem /= 5ULL;
    }
    while (rem % 7ULL == 0) {
        cnt[3]++;
        rem /= 7ULL;
    }

    if (rem != 1)          /* contains prime > 7 */
        return 0;

    /* Build the largest number by concatenating digits in descending order:
         7, 5, 3, 2                                */
    const int digit[4] = {2, 3, 5, 7};

    seq_t result = 0;
    for (int i = 3; i >= 0; --i) {
        for (uint64_t k = 0; k < cnt[i]; ++k) {
            result = result * 10 + (seq_t)digit[i];
        }
    }
    return result;
}
