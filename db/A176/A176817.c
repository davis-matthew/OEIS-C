/*
 * A176817  Nonprimes k such that 2^(k-1) == 4^(k-1) (mod k).
 * https://oeis.org/A176817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th nonprime integer k such that 2^(k-1) == 4^(k-1)
 *            (mod k)
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k)
    {
        if (k == 1) {                    // 1 is counted as a nonprime
            ++count;
            if (count == n) return (seq_t)k;
            continue;
        }
        if (is_prime(k))                 // skip primes
            continue;
        if ((k & (k - 1)) == 0) {        // power of two
            if (k > 2) {                 // 2 is prime, so exclude it
                ++count;
                if (count == n) return (seq_t)k;
            }
            continue;
        }
        /* For composite non‑powers‑of‑two, test the odd part.
           k = 2^a * m  (m odd). The condition reduces to
           2^(k-1) ≡ 1 (mod m). */
        idx_t m = k;
        while ((m & 1) == 0) m >>= 1;
        seq_t r = mod_pow((seq_t)2, k - 1, (seq_t)m);
        if (r == (seq_t)1) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
    return 0;    // never reached
}
