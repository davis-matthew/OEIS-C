/*
 * A091787  a(1) = 2. To get a(n+1), write the string a(1)a(2)...a(n) as xy^k
 *          for words x and y (where y has positive length) and k is
 *          maximized, i.e., k = the maximal number of repeating blocks at the
 *          end of the sequence so far. Then a(n+1) = max(k,2).
 * https://oeis.org/A091787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(1)=2; for n≥1, let S=a(1)…a(n). Write S=xy^k with y≠ε, k
 *            maximised. Then a(n+1)=max(k,2). For a sequence stored as an
 *            array of terms we find the largest k such that the suffix of S
 *            is a repetition of some block y. The routine below does this by
 *            brute‑force search of block lengths, but with a si...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used, harmless */
    /* allocate space for the sequence up to n terms */
    seq_t *seq = (seq_t *)malloc(n * sizeof(seq_t));
    if (!seq) return 0;                  /* allocation failed (unlikely) */

    seq[0] = 2;                          /* a(1)=2 */
    if (n == 1) { free(seq); return 2; }

    /* compute subsequent terms */
    for (idx_t i = 1; i < n; ++i) {      /* i is current length of known prefix */
        idx_t len = i;                    /* current length of S = a[0..len-1] */
        idx_t max_k = 1;                  /* k is at least 1 */

        /* search block lengths L from 1 upwards.  If len/L ≤ max_k, further Ls
           cannot beat max_k, so we stop early.                                    */
        for (idx_t L = 1; L <= len / 2 && len / L > max_k; ++L) {
            /* position of start of last block y */
            idx_t pos_start = len - L;
            idx_t k = 1;                   /* at least one block (the final one) */

            /* compare previous blocks of length L with the current block */
            while (pos_start >= L) {
                int equal = 1;
                for (idx_t j = 0; j < L; ++j) {
                    if (seq[pos_start - L + j] != seq[pos_start + j]) {
                        equal = 0; break;
                    }
                }
                if (!equal) break;
                ++k;
                pos_start -= L;
            }

            if (k > max_k) max_k = k;
        }

        /* compute new term a[i+1] = max(k,2) */
        seq[i] = (max_k > 1 ? max_k : 2);
    }

    seq_t result = seq[n-1];
    free(seq);
    return result;
}
