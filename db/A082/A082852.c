/*
 * A082852  a(0)=0, a(n) = A014137(A072643(n)-1).
 * https://oeis.org/A082852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014137(A072643(n)-1) Namely a(n) = T(k-1) where T(k) =
 *            ∑_{i=0}^{k-1} Catalan(i) and k is the smallest index with T(k) >
 *            n
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n) {
    seq_t cum   = 0;                 /* value T(k-1) for current block */
    seq_t n_seq = (seq_t)n;          /* promote index for comparison */

    for (idx_t k = 0; ; ++k) {
        seq_t cat  = catalan(k);     /* Catalan(k) */
        seq_t next = cum + cat;      /* T(k) = cum + Catalan(k) */

        if (n_seq < next) {
            return cum;              /* n lies in block of size Catalan(k) */
        }
        cum = next;                  /* proceed to next block */
    }
}
