/*
 * A145838  Primes in A005891 = Centered pentagonal numbers: (5n^2 + 5n +
 *          2)/2.
 * https://oeis.org/A145838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime centered pentagonal number where centered
 *            pentagonal numbers are C(k) = (5*k^2 – 5*k + 2)/2. The function
 *            enumerates k = 0, 1, 2, …, checks each C(k) for primality, and
 *            returns the n‑th one found. Unchecked for n = 0 (returns 0)
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 26, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined case */

    idx_t count = 0;
    idx_t k = 0;
    seq_t val = 0;

    while (count < n) {
        seq_t kk = (seq_t)k;
        /* C(k) = (5*k^2 - 5*k + 2)/2 */
        val = (((seq_t)5 * kk * kk) - ((seq_t)5 * kk) + (seq_t)2) / 2;

        if (is_prime_big(val)) {
            ++count;
        }
        ++k;
    }

    return val;
}
