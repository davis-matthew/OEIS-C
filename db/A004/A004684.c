/*
 * A004684  Primes written in base 11. (Next term contains a nondecimal
 *          character.)
 * https://oeis.org/A004684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   search -- a(n) = n-th prime p (taken in increasing order) whose
 *            base-11 representation uses only digits 0..9, read as a decimal
 *            number. The sequence is finite: the next prime, 43 = 3A_11,
 *            contains a nondecimal digit
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   a004684_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a004684_isprime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p;

    for (p = 2;; p++) {
        uint64_t t;
        bool ok = true;

        if (!a004684_isprime(p)) continue;

        for (t = p; t > 0; t /= 11) {
            if (t % 11 > 9) { ok = false; break; }
        }
        if (!ok) continue;

        count++;
        if (count == n) {
            uint64_t digits[32];
            int k = 0, i;
            seq_t result = 0;
            for (t = p; t > 0; t /= 11) digits[k++] = t % 11;
            for (i = k - 1; i >= 0; i--) {
                result *= 10;
                result += (seq_t)digits[i];
            }
            return result;
        }
    }
}
