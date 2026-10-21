/*
 * A204933  The index j<k such that n divides k!-j!, where k is the least
 *          index (A204932) for which such j exists.
 * https://oeis.org/A204933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A204933 – find the smallest j < k such that n | k! – j!,
    where k is the first index for which such a j exists.
    The algorithm stores each factorial residue modulo n as it
    computes them.  The first repetition of a residue yields the
    required j.  This guarantees the minimal k and minimal j. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1 directly – the loop below
       would also work but this is explicit. */
    if (n == 1) return (seq_t)1;

    /* Allocate an array that records, for each residue r (0..n-1),
       the first index k at which k! ≡ r (mod n).  0 means "not seen". */
    idx_t *first = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!first) return (seq_t)0;          /* out‑of‑memory guard */

    for (idx_t i = 0; i < n; ++i) first[i] = 0;

    /* Compute factorial residues modulo n. */
    seq_t fac = 1 % (seq_t)n;            /* 1! mod n */
    first[(idx_t)fac] = 1;               /* j = 1 is the first occurrence */

    for (idx_t k = 2; ; ++k)
    {
        fac = (fac * (seq_t)k) % (seq_t)n;   /* k! mod n */
        idx_t r = (idx_t)fac;

        /* If this residue was seen before, we found the minimal k
           and the corresponding j. */
        if (first[r] != 0)
        {
            idx_t j = first[r];
            free(first);
            return (seq_t)j;
        }

        /* Record the earliest occurrence of this residue. */
        first[r] = k;
    }
}
