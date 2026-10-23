/*
 * A104275  Numbers k such that 2k-1 is not prime.
 * https://oeis.org/A104275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 159, u16 n <= 7536, u32 n <= 7536, u64 n <= 7536,
 *            u128 n <= 7536, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the smallest integer k such that 2k-1 is not prime
   (i.e., 2k-1 is composite or equals 1).  a(1)=1, a(2)=5, a(3)=8, ... */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1: n>=1 in tests */
    idx_t k = 1;
    idx_t count = 0;
    while (1) {
        uint64_t odd = (uint64_t)k * 2ull - 1ull;  /* 2k-1, fits in 64‑bit for test range */
        if (!is_prime(odd)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
