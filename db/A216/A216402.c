/*
 * A216402  Least prime p such that p = n (mod 59).
 * https://oeis.org/A216402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least prime p such that p ≡ n (mod 59). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t minPrimeForResidue[59] = {0};

/* Compute the least prime congruent to each residue 0..58 modulo 59.
   0 corresponds to the prime 59 itself. */
static void init_residues(void)
{
    for (unsigned r = 0; r < 59; ++r)
    {
        if (r == 0)
        {
            minPrimeForResidue[r] = 59;          /* 59 ≡ 0 (mod 59) */
            continue;
        }
        uint64_t cand = r;
        while (!is_prime(cand))
            cand += 59;
        minPrimeForResidue[r] = (seq_t)cand;
    }
}

seq_t a(idx_t n)
{
    /* Modulo 59 residue of n */
    unsigned r = (unsigned)(n % 59ULL);

    static bool initialized = false;
    if (!initialized)
    {
        init_residues();
        initialized = true;
    }

    return minPrimeForResidue[r];
}
