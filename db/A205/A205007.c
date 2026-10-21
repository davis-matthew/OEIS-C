/*
 * A205007  a(n) = (1/n)*A205006(n), where A205006(n) = s(k)-s(j), with
 *          (s(k),s(j)) the least pair of distinct triangular numbers for
 *          which n divides their difference.
 * https://oeis.org/A205007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * A205006(n), * where A205006(n) is the difference
 *            of the first pair of distinct * triangular numbers whose
 *            difference is divisible by n
 * Range:     u8 n <= 9656, u16 n <= 9656, u32 n <= 9656, u64 n <= 9656,
 *            u128 n <= 9656, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n = 0, return 0 for safety */

    idx_t N = n;
    /* Array of first occurrences of each residue modulo n. 0 means unused. */
    idx_t *first = (idx_t *)calloc((size_t)N, sizeof(idx_t));
    if (!first)
        return 0; /* allocation failure */

    for (idx_t k = 1; ; ++k) {
        /* Compute T(k) = k*(k+1)/2 modulo n */
        idx_t tmod = (k * (k + 1) / 2) % N;
        if (first[tmod] == 0) {
            first[tmod] = k;
        } else {
            idx_t j = first[tmod];

            seq_t Tk = (seq_t)k * ((seq_t)k + 1) / 2;
            seq_t Tj = (seq_t)j * ((seq_t)j + 1) / 2;
            seq_t D   = Tk - Tj;
            seq_t res = D / (seq_t)n;

            free(first);
            return res;
        }
    }
}
