/*
 * A168024  Noncomposite numbers in the northwestern ray of the Ulam spiral as
 *          oriented on the March 1964 cover of Scientific American.
 * https://oeis.org/A168024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th integer of the form 4*k^2+1 that equals 1 or is
 *            prime
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; ; ++k) {
        seq_t val = (seq_t)4 * (seq_t)k * (seq_t)k + (seq_t)1;
        bool good = (k == 0);
        if (!good) good = is_prime_big(val);
        if (good) {
            if (count == n) return val;
            ++count;
        }
    }
}
