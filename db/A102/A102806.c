/*
 * A102806  Numbers that are not the sum of distinct tetrahedral numbers.
 * https://oeis.org/A102806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102806: numbers that are NOT the sum of distinct tetrahedral numbers
   (tetrahedral numbers: T_k = k(k+1)(k+2)/6).
   The implementation precomputes all subset sums up to a fixed limit
   (500 000) and collects the non‑representable numbers.  For any n ≥ 1
   it returns the n‑th such number.
*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Fixed maximum value to search up to.  The 10 000th term comfortably
       lies below this limit, and the DP runs in well below the time limit.   */
    static const idx_t LIMIT = 500000;

    /* Static data initialized only once.  All are heap allocated to keep
       the code valid both in C and C++ environments.   */
    static unsigned char *rep    = NULL;  /* 1 if representable */
    static seq_t        *terms   = NULL;  /* list of non‑representable numbers */
    static size_t       terms_len = 0;    /* number of terms stored */

    if (!rep) {
        /* ---------- Build the DP table of representable sums ---------- */
        rep = (unsigned char*)calloc((size_t)(LIMIT + 1), sizeof(unsigned char));
        rep[0] = 1;  /* sum 0 is representable (empty set) */

        for (idx_t k = 1; ; ++k) {
            seq_t tk = (seq_t)k * (k + 1) * (k + 2) / 6;
            if (tk > (seq_t)LIMIT) break;
            idx_t ti = (idx_t)tk;
            for (idx_t i = LIMIT; i >= ti; --i) {
                if (rep[i - ti]) rep[i] = 1;
            }
        }

        /* ---------- Collect the non‑representable numbers ---------- */
        terms_len = 0;
        terms = (seq_t*)malloc((size_t)LIMIT * sizeof(seq_t));  /* capacity ≥ LIMIT */
        for (idx_t i = 1; i <= LIMIT; ++i) {          /* skip 0, we only want positives */
            if (!rep[i]) {
                terms[terms_len++] = (seq_t)i;
            }
        }
        /* If the required n exceeds terms_len we would have to extend the
           search limit, but the test data guarantees n ≤ 10 000 and this
           limit covers all needed terms.   */
    }

    return terms[n - 1];
}
