/*
 * A141849  Primes congruent to 1 mod 11.
 * https://oeis.org/A141849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 494, u32 n <= 543, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the nth prime congruent to 1 (mod 11) by brute‑force search.
    a(1) = 23, a(2) = 67, a(3) = 89, …  */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;       /* offset is 1 – behaviour for n<1 is irrelevant */

    idx_t count = 0;
    for (idx_t k = 2;; ++k) {   /* candidates: 11*k + 1  (k ≥ 2 → 23, 67, …) */
        seq_t cand = 11 * (seq_t)k + 1;
        if (is_prime_big(cand)) {
            ++count;
            if (count == n) return cand;
        }
    }
}
