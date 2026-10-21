/*
 * A205406  a(n) = s(k)-s(j), where (k,j) is the least pair for which n
 *          divides s(k)-s(j), and s(j) = floor((j+1)^2/2)/2.
 * https://oeis.org/A205406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 9356, u32 n <= 9415, u64 n <= 9415,
 *            u128 n <= 9415, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for OEIS A205406.
   a(n) = s(k) - s(j) where (k,j) is the minimal pair with 1 ≤ j < k ≤ n+1
   and (s(k)-s(j)) mod n == 0.  s(x) = floor((x+1)^2/4).
   Brute‑force search over k up to n+1 guarantees a solution. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* idx_t and seq_t are provided by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* out of OEIS range */
        return (seq_t)0;

    /* Allocate arrays to remember the first s(j) for each residue modulo n. */
    bool *seen = (bool *)malloc((size_t)n * sizeof(bool));
    if (!seen)
        abort();

    idx_t *first_s = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!first_s) {
        free(seen);
        abort();
    }

    memset(seen, 0, (size_t)n * sizeof(bool));

    /* Scan s(k) for k = 1 .. n+1.  The pigeonhole principle guarantees
       that we will encounter a repeated residue within this range. */
    for (idx_t k = 1; k <= n + 1; ++k) {
        idx_t kj = k + 1;             /* (k+1) for the formula */
        idx_t sq = kj * kj;           /* (k+1)^2 -- fits in 64 bits for n ≤ 10000 */
        idx_t s_k = sq / 4;           /* floor((k+1)^2/4) */
        idx_t res = s_k % n;          /* residue modulo n */

        if (!seen[res]) {
            seen[res] = true;
            first_s[res] = s_k;
        } else {
            idx_t diff = s_k - first_s[res];  /* s(k)-s(j) */
            seq_t result = (seq_t)diff;
            free(seen);
            free(first_s);
            return result;
        }
    }

    /* Should never reach here (pigeonhole principle). */
    free(seen);
    free(first_s);
    return (seq_t)0;
}
