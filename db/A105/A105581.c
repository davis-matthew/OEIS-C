/*
 * A105581  Primes whose indices are palindromic.
 * https://oeis.org/A105581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = prime(p) where p is the n‑th palindromic integer
 *            (starting at 1)
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   get_prime, reverse_without_last
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/reverse_without_last.h"

static idx_t make_pal(idx_t half, int odd) {
    idx_t pal = half;
    idx_t rev = reverse_without_last(half, odd);
    while (rev > 0) {
        pal = pal * 10 + rev % 10;
        rev /= 10;
    }
    return pal;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n < 1, return 0 */

    idx_t count = 0;
    for (int len = 1; ; ++len) {
        if (len == 1) {
            /* 1–9 are the 1‑digit palindromes */
            for (idx_t half = 1; half <= 9; ++half) {
                ++count;
                if (count == n) return get_prime(half);
            }
        } else {
            int half_len = (len + 1) / 2;
            idx_t start = 1;
            for (int i = 1; i < half_len; ++i) start *= 10;   /* first half cannot start with 0 */
            idx_t end = start * 10 - 1;
            int odd = len % 2;   /* 1 if odd, 0 if even */
            for (idx_t half = start; half <= end; ++half) {
                idx_t pal = make_pal(half, odd);
                ++count;
                if (count == n) return get_prime(pal);
            }
        }
    }
}
