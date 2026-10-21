/*
 * A054714  Number of powers of 14 modulo n.
 * https://oeis.org/A054714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct residues of 14^k mod n (k ≥ 0)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* safety guard */
    /* Boolean array to remember seen residues */
    bool *seen = (bool*)calloc((size_t)n, sizeof(bool));
    if (!seen) return 0;                   /* memory allocation failed */

    seq_t cur   = (seq_t)1 % (seq_t)n;     /* 14^0 mod n */
    seq_t count = 0;

    while (!seen[(size_t)cur]) {
        seen[(size_t)cur] = true;
        count++;
        cur = (cur * (seq_t)14) % (seq_t)n;
    }

    free(seen);
    return count;
}
