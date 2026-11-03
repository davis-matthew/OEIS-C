/*
 * A100203  Primes of the form 37n+3.
 * https://oeis.org/A100203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form 37*k+3
 * Range:     u8 n <= 2, u16 n <= 186, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 801 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 */

    idx_t count = 0;
    idx_t k = 0;
    while (true) {
        seq_t candidate = (seq_t)k * (seq_t)37 + (seq_t)3;

        bool prime;
        if (candidate <= (seq_t)UINT64_MAX) {
            prime = is_prime_u64((uint64_t)candidate);
        } else {
            prime = is_prime_seq(candidate);
        }

        if (prime) {
            count = count + 1;
            if (count == n) return candidate;
        }
        k = k + 1;
    }
}
