/*
 * A054705  Number of powers of 4 modulo n.
 * https://oeis.org/A054705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 521, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of distinct residues among powers of 4 modulo n
 *
 * Computes the size of the set {4^k mod n : k >= 0}.  Works for any n >= 1.
 * For n = 1 the set is {0} so the result is 1.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n starts from 1; guard against an illegal 0 for safety */
    if (n == 0)
        return 0;

    /* Allocate a visited array of n bytes (one per residue). */
    unsigned char *visited =
        (unsigned char *)calloc((size_t)n, sizeof(unsigned char));
    if (!visited)          /* out of memory – return 0 as a neutral value */
        return 0;

    seq_t count = 0;
    seq_t cur = 1 % (seq_t)n;           /* 4^0 mod n */

    /* Iterate until a residue repeats. */
    while (!visited[(size_t)cur]) {
        visited[(size_t)cur] = 1;       /* mark this residue visited */
        count++;
        cur = (cur * (seq_t)4) % (seq_t)n;   /* next power of 4 modulo n */
    }

    free(visited);
    return count;
}
