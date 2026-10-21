/*
 * A073119  Total number of parts which are positive powers of 2 in all
 *          partitions of n.
 * https://oeis.org/A073119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{s=2^i , i≥1} Σ_{t=1}^{⌊n/s⌋} p(n – t·s) where p(m) is
 *            the partition function A000041
 * Range:     u8 n <= 14, u16 n <= 36, u32 n <= 111, u64 n <= 375,
 *            u128 n <= 1360, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Returns the total number of parts which are positive powers of 2
   in all partitions of n (OEIS A073119). */
seq_t a(idx_t n) {
    static seq_t *p = NULL;       /* partition numbers 0 .. max_n */
    static seq_t *aarr = NULL;    /* required sequence values 0 .. max_n */
    static idx_t max_n = 0;       /* largest n that has been computed */

    if (n <= 0) /* the sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* Extend the precomputed tables if necessary. */
    if (n > max_n) {
        idx_t new_n = n;

        /* Allocate storage for partitions and the answer array. */
        seq_t *p_new = (seq_t*)calloc(new_n + 1, sizeof(seq_t));
        seq_t *a_new = (seq_t*)calloc(new_n + 1, sizeof(seq_t));

        /* p[0] = 1; others are already zero by calloc. */
        p_new[0] = (seq_t)1;

        /* Compute partition numbers p[0..new_n] by the standard
           additive dynamic programming: p[j] += p[j-i] for i ≤ j. */
        for (idx_t i = 1; i <= new_n; ++i) {
            for (idx_t j = i; j <= new_n; ++j) {
                p_new[j] += p_new[j - i];
            }
        }

        /* Compute a_new[0..new_n] using the efficient summation over
           powers of two. For each power s = 2,4,8,… we maintain a
           temporary array f where f[m] = f[m‑s] + p_new[m‑s] for m ≥ s.
           Then a_new[m] += f[m] for all m. */
        for (idx_t s = 2; s <= new_n; s <<= 1) {
            seq_t *f = (seq_t*)calloc(new_n + 1, sizeof(seq_t));
            for (idx_t m = s; m <= new_n; ++m) {
                f[m] = f[m - s] + p_new[m - s];
                a_new[m] += f[m];
            }
            free(f);
        }

        /* Replace the old tables. */
        if (p)   free(p);
        if (aarr) free(aarr);

        p = p_new;
        aarr = a_new;
        max_n = new_n;
    }

    return aarr[n];
}
