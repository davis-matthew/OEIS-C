/*
 * A238849  Smallest k such that k*n^3 - 1 and k*n^3 + 1 are twin primes.
 * https://oeis.org/A238849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 12, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal k such that k*n³ – 1 and k*n³ + 1 are twin primes. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t nseq = (seq_t)n;
    seq_t n3 = nseq * nseq * nseq;           /* n³ */

    bool need_even = (n3 & 1) != 0;            /* n³ odd ⇒ k must be even */
    seq_t k = need_even ? (seq_t)2 : (seq_t)1;
    seq_t step = need_even ? (seq_t)2 : (seq_t)1;

    /* Search for the smallest k satisfying the twin‑prime condition. */
    for (;;)
    {
        seq_t p1 = n3 * k - (seq_t)1;
        seq_t p2 = n3 * k + (seq_t)1;
        if (is_prime_big(p1) && is_prime_big(p2))
            return k;
        k += step;
    }
}
