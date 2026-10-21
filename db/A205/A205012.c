/*
 * A205012  Least s(k) such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=C(2j-2,j-1).
 * https://oeis.org/A205012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   least s(k) such that n divides s(k)-s(j) for some j<k, where
 *            s(j)=C(2j-2,j-1). a(1) = 2, a(2) = 6, a(3) = 20,
 * Range:     u8 n <= 7, u16 n <= 30, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(1) is the first defined term; for safety return 0 for n==0 */
    if (n == 0) return 0;

    /* allocate an array of size n to remember which residues have appeared */
    size_t sz = (size_t)n;
    bool *seen = (bool*)calloc(sz, sizeof(bool));
    if (!seen) return 0;               /* out‑of‑memory: return 0 as a safe default */

    /* s(1) = C(0,0) = 1 */
    seq_t s = 1;
    idx_t r = (idx_t)(s % (seq_t)n);
    seen[r] = true;

    /* current index j = 1 (so we use j in the recurrence for the next step) */
    idx_t j = 1;

    while (1) {
        /* advance to the next s(k) using the recurrence
           s(k+1) = s(k) * 2*(2k-1) / k, here k=j            */
        ++j;
        seq_t mult = (seq_t)2 * (seq_t)(2*j - 3);  /* 2*(2j-3) */
        s = s * mult;
        s = s / (seq_t)(j - 1);

        r = (idx_t)(s % (seq_t)n);
        if (seen[r]) {
            free(seen);
            return s;       /* first repeated residue found */
        }
        seen[r] = true;
    }
}
