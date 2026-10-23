/*
 * A147573  Numbers with exactly 6 distinct prime divisors {2,3,5,7,11,13}.
 * https://oeis.org/A147573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30030 * A080197(n)
 * Range:     u8 none, u16 n <= 2, u32 n <= 2108, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th number having exactly the six distinct prime
 * divisors 2,3,5,7,11,13.  The sequence equals 30030 times the
 * 6‑prime Hamming sequence: 1,2,3,4,5,6,7,8,9,...
 * This implementation uses the classic pointer method and
 * allocates an array of size n to generate the terms. */

#include <stdlib.h>
#include <stdint.h>

static const uint64_t primes[6] = {2, 3, 5, 7, 11, 13};

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* value below the offset is irrelevant */

    /* Allocate an array to keep the first n Hamming numbers. */
    size_t len = (size_t)n;
    seq_t *smooth = (seq_t *)malloc(len * sizeof(seq_t));
    if (!smooth) return 0;          /* allocation failure */

    smooth[0] = (seq_t)1;           /* the multiplicative identity */

    /* Pointers into the smooth array for each prime. */
    size_t ptr[6] = {0, 0, 0, 0, 0, 0};

    /* Generate the 6‑prime Hamming sequence up to the n‑th term. */
    for (size_t i = 1; i < len; ++i)
    {
        /* Find the minimal candidate produced by each prime. */
        seq_t next = (seq_t)primes[0] * smooth[ptr[0]];
        for (int j = 1; j < 6; ++j)
        {
            seq_t cand = (seq_t)primes[j] * smooth[ptr[j]];
            if (cand < next) next = cand;
        }
        smooth[i] = next;

        /* Advance all pointers that contributed to the minimum to avoid duplicates. */
        for (int j = 0; j < 6; ++j)
        {
            seq_t cand = (seq_t)primes[j] * smooth[ptr[j]];
            if (cand == next) ++ptr[j];
        }
    }

    seq_t result = smooth[len - 1] * (seq_t)30030;  /* scale by the fixed factor */

    free(smooth);          /* release the temporary array */
    return result;
}
