/*
 * A204931  Least k such that n divides A204930(k), the k-th difference of two
 *          distinct factorials.
 * https://oeis.org/A204931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n divides A204930(k) where A204930(k)
 *            is the k‑th difference i! - j! (i>j≥1) enumerated by increasing
 *            i and, for each i, increasing j. Computation: search factorial
 *            residues modulo n; the first repeated residue yields the desired
 *            difference
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Allocate array storing the first i such that i! ≡ r (mod n) */
    size_t m = (size_t)n;
    idx_t *seen = (idx_t*)calloc(m, sizeof(idx_t));
    if (!seen)
        return 0;   /* allocation failure */

    seq_t mod = (seq_t)n;
    seq_t res = ((seq_t)1) % mod;      /* 1! mod n  (i = 1) */
    seen[(size_t)res] = 1;             /* record residue for i = 1 */

    for (idx_t i = 2;; ++i) {
        res = (res * (seq_t)i) % mod;   /* i! mod n */

        idx_t j = seen[(size_t)res];
        if (j != 0) {                  /* repeat found → i! ≡ j! (mod n) */
            seq_t i_seq = (seq_t)i;
            seq_t k = ((i_seq - (seq_t)2) * (i_seq - (seq_t)1)) / (seq_t)2
                      + (seq_t)j;
            free(seen);
            return k;
        }
        seen[(size_t)res] = i;          /* store first occurrence */
    }
}
