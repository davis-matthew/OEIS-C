/*
 * A196445  Numbers k >= 2 such that A055035(k) is an odd integer.
 * https://oeis.org/A196445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * m where m is the n‑th odd integer such that * m = 1
 *            or φ(m) ≡ 2 (mod 4). This gives the even numbers k = 2m * with
 *            φ(k/2) ≡ 2 (mod 4), which are the terms of A196445
 * Range:     u8 n <= 22, u16 n <= 1803, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count of sequence terms found so far. */
    idx_t found = 0;
    /* Current odd m to test (k = 2m). */
    idx_t m = 1;

    while (1) {
        /* Compute Euler’s totient φ(m).  m is odd, so we trial‑divide
         * by odd divisors only.  φ(m) is stored in an unsigned 64‑bit
         * integer; this is enough for all m that appear when n ≤ 10000
         * (the k values are far below 2^64). */
        uint64_t phi = m;
        idx_t tmp = m;
        for (idx_t p = 3; p <= tmp / p; p += 2) {
            if (tmp % p == 0) {
                /* Remove the prime factor p from tmp. */
                while (tmp % p == 0) tmp /= p;
                phi -= phi / p;
            }
        }
        if (tmp > 1) {
            /* tmp is a remaining prime factor > sqrt(m). */
            phi -= phi / tmp;
        }

        /* Test the condition: m = 1 or φ(m) ≡ 2 (mod 4). */
        if (m == 1 || ((phi & 3u) == 2u)) {
            found++;
            if (found == n) {
                /* Return the sequence term 2*m as a seq_t value. */
                return (seq_t)2 * (seq_t)m;
            }
        }

        /* Move to the next odd integer. */
        m += 2;
    }
}
