/*
 * A205131  The index j<k such that n divides s(k)-s(j), where k is the least
 *          index (A205130) for which such j exists, and s(k)=2*k^2-k, the
 *          k-th hexagonal number.
 * https://oeis.org/A205131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the smallest index j < k such that n | (s(k)-s(j)),
   where k is the smallest index for which such a j exists.
   s(k) = 2*k^2 - k  (the k-th hexagonal number). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n must be ≥1 (offset 1). For n==0 we simply return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Array that stores the first occurrence of each residue mod n.
       0 means “not seen yet”.  The array size is n, which is
       fine for the usual bounds of the sequence (n ≤ 10^4 or so). */
    idx_t *first = (idx_t *)calloc(n, sizeof(idx_t));
    if (first == NULL)
        return (seq_t)0;          /* allocation failed; no result */

    idx_t k = 1;                     /* current index k */
    idx_t s_mod = 1 % n;             /* s(1) modulo n */
    first[s_mod] = k;                /* record first occurrence */

    for (;;) {
        ++k;                         /* next k */

        /* Increment s(k) = s(k-1) + (4*(k-1)+1) modulo n. */
        idx_t km1_mod = (k - 1) % n;
        idx_t inc_mod = ((4 * km1_mod) % n + 1) % n;
        s_mod = (s_mod + inc_mod) % n;

        if (first[s_mod] == 0) {
            /* First time this residue occurs. */
            first[s_mod] = k;
        } else {
            /* Repeat found – this is the minimal k. */
            idx_t j = first[s_mod];
            free(first);
            return (seq_t)j;          /* convert index to seq_t */
        }
    }
}
