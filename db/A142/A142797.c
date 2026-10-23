/*
 * A142797  Primes congruent to 49 mod 60.
 * https://oeis.org/A142797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 49 modulo 60, found by
 *            incremental search
 * Range:     u8 n <= 2, u16 n <= 392, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* the sequence is 1‑based */
    if (n == 0)
        return 0;

    idx_t cand = 49;          /* start at 49 ≡ 49 (mod 60) */
    idx_t found = 0;

    while (found < n) {
        cand += 60;            /* next candidate with residue 49 mod 60 */
        if (is_prime(cand))
            ++found;
    }
    return (seq_t)cand;
}
