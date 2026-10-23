/*
 * A161781  Binary encodings of prime constellations.
 * https://oeis.org/A161781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1342, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A161781: Binary encodings of prime constellations
 *
 * A constellation is described by the set of even differences
 * from a prime p: d_i = 2*t_i, where the t_i's are the integers
 * represented by the bit positions of an odd integer e.
 *
 * The encoding of a constellation is the sum of 2^t_i, which is the
 * integer e itself.  The sequence lists all admissible constellations,
 * sorted by increasing encoding value e.  A constellation is
 * admissible iff for every odd prime p not exceeding the number of
 * bits set in e, the set of residues {t_i mod p} does not contain all
 * residues 0..p-1.  (Equivalently, the set of residues of the
 * differences modulo p does not cover all residues.)
 *
 * The implementation scans odd integers e, tests admissibility,
 * and returns the nth valid encoding (0‑based).  Only a small list
 * of odd primes up to 31 is needed, because the number of
 * set bits in e is at most the number of its binary digits.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* odd primes needed for admissibility test (p <= |t_i|) */
static const unsigned int odd_primes[] = {
    3, 5, 7, 11, 13, 17, 19, 23, 29, 31
};

seq_t a(idx_t n)
{
    /* target number of valid encodings to find (n is 0‑based) */
    idx_t target = n + 1;
    idx_t found  = 0;

    /* iterate over odd integers only, since all valid encodings
       must contain the 0th bit (t = 0). */
    for (idx_t e = 1; ; e += 2)
    {
        /* collect the positions of set bits in e */
        unsigned int k = 0;                 /* number of set bits */
        unsigned int pos[64];                /* enough for 64‑bit numbers */
        idx_t t = e;
        while (t) {
            unsigned int bit = __builtin_ctzll((unsigned long long)t);
            pos[k++] = bit;
            t &= t - 1;                     /* clear lowest set bit */
        }

        /* admissibility test */
        bool admissible = true;
        for (size_t pi = 0; pi < sizeof(odd_primes)/sizeof(odd_primes[0])
                            && odd_primes[pi] <= k; ++pi)
        {
            unsigned int p = odd_primes[pi];
            uint64_t mask = 0;
            for (unsigned int j = 0; j < k; ++j)
                mask |= 1ULL << (pos[j] % p);

            if (mask == ((1ULL << p) - 1)) {   /* all residues covered */
                admissible = false;
                break;
            }
        }

        if (!admissible)
            continue;   /* skip this e */

        ++found;
        if (found == target)
            return (seq_t)e;    /* cast to sequence element type */
    }
}
