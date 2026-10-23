/*
 * A193279  Number of distinct sums of distinct proper divisors of n.
 * https://oeis.org/A193279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct sums of distinct proper divisors of n
 *            (non‑empty subsets)
 * Range:     u8 n <= 143, u16 n <= 9725, u32 n <= 9725, u64 n <= 9725,
 *            u128 n <= 9725, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 1) return 0;

    /* collect proper divisors and their sum */
    uint64_t divs[256];
    int divcnt = 0;
    uint64_t total_sum = 0;

    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            uint64_t d1 = i, d2 = n / i;
            if (d1 != n) {
                divs[divcnt++] = d1;
                total_sum += d1;
            }
            if (d2 != n && d2 != d1) {
                divs[divcnt++] = d2;
                total_sum += d2;
            }
        }
    }

    /* bitset DP for subset sums */
    size_t bits  = total_sum + 1;
    size_t words = (bits + 63) / 64;
    uint64_t *bs = (uint64_t *)malloc(words * sizeof(uint64_t));
    if (!bs) return 0; /* fallback if allocation fails */
    memset(bs, 0, words * sizeof(uint64_t));
    bs[0] = 1ULL; /* sum 0 reachable */

    for (int k = 0; k < divcnt; ++k) {
        uint64_t d = divs[k];
        size_t wshift = d / 64;
        unsigned bshift = d % 64;

        for (size_t i = words; i-- > wshift; ) {
            uint64_t val = bs[i - wshift] << bshift;
            if (bshift && (i - wshift) != 0) {
                val |= bs[i - wshift - 1] >> (64 - bshift);
            }
            bs[i] |= val;
        }
    }

    /* count reachable sums excluding 0 */
    seq_t cnt = 0;
    for (size_t i = 0; i < words; ++i) {
        uint64_t w = bs[i];
        if (i == 0) w &= ~1ULL; /* clear bit 0 */
        cnt += (seq_t)__builtin_popcountll(w);
    }

    free(bs);
    return cnt;
}
