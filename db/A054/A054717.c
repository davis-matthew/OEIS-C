/*
 * A054717  Number of powers of 9 modulo n.
 * https://oeis.org/A054717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of distinct powers of 9 modulo n
   a(n) = |{ 9^k mod n : k ≥ 0 }|   for n ≥ 1
   Implementation: iterate powers of 9 modulo n,
   marking appeared residues until a repeat occurs. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, though n ≥ 1 per definition */

    /* Allocate a visited array of size n. */
    size_t sz = (size_t)n;
    unsigned char *seen = (unsigned char *)calloc(sz, 1);
    if (!seen) return 0;     /* allocation failure → return 0 */

    seq_t count = 0;
    seq_t val   = 1 % (seq_t)n;   /* 9^0 mod n */

    while (!seen[(size_t)val]) {
        seen[(size_t)val] = 1;
        count++;
        val = (val * 9) % (seq_t)n;
    }

    free(seen);
    return count;
}
