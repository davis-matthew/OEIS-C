/*
 * A070012  Floor of number of prime factors of n divided by the number of n's
 *          distinct prime factors.
 * https://oeis.org/A070012
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(Ω(n) / ω(n)), where Ω(n) is the total number of
 *            prime factors of n counted with multiplicity and ω(n) is the
 *            number of distinct prime factors of n (n ≥ 2)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple trial‑division factorization.
   It is perfectly adequate for n up to about 10 000 as required
   in the problem statement. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* value for offsets < 2 is irrelevant */

    idx_t t = n;
    seq_t totalExp = 0;   /* Ω(n) */
    seq_t distinct  = 0;   /* ω(n) */

    /* factor 2 separately */
    if ((t & 1) == 0) {
        ++distinct;
        while ((t & 1) == 0) {
            t >>= 1;
            ++totalExp;
        }
    }

    /* odd primes */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            ++distinct;
            while (t % p == 0) {
                t /= p;
                ++totalExp;
            }
        }
    }

    /* remaining prime factor (if any) */
    if (t > 1) {
        ++distinct;
        ++totalExp;
    }

    return totalExp / distinct;   /* integer division gives floor */
}
