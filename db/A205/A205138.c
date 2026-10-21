/*
 * A205138  Least k such that n divides s(k)-s(j) for some j satisfying
 *          1<=j<k, where s(j)=j(3j-1)/2, the j-th pentagonal number.
 * https://oeis.org/A205138
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

/* Sequence A205138: a(n) is the least k such that n | (s(k)-s(j)) for some
   1 ≤ j < k, where s(j) = j(3j-1)/2 is the j‑th pentagonal number.
   Implementation: iterate over k, keep a table of seen residues
   of s(k) modulo n, and return the first k that repeats a residue. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 (for n = 0 the sequence is undefined) */
    if (n == 0) return 0;

    /* table of first occurrence of each residue modulo n;
       0 means “not seen yet” – indices start at 1 */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first) return 0;  /* allocation failure */

    idx_t k = 1;
    /* s(1) = 1, so remainder starts as 1 % n */
    idx_t rem = 1 % n;

    while (1) {
        if (first[rem] != 0) {
            free(first);
            return (seq_t)k;   /* repetition found */
        }
        first[rem] = k;

        /* Prepare for next k */
        ++k;
        /* Incremental update: s(k) = s(k-1) + 3(k-1) + 1 */
        idx_t incr = ((idx_t)3 % n) * ((k - 1) % n) + 1;
        rem += incr;
        if (rem >= n) rem %= n;
    }
}
