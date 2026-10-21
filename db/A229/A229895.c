/*
 * A229895  k-tuple sizes in A229873.
 * https://oeis.org/A229895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the length of the n‑th tuple in the canonical
 *            enumeration of all finite tuples of positive integers (A229873).
 *            This enumeration lists, for each m = 1,2,3,…, all tuples of
 *            length L = 1,…,m whose entries lie in {1,…,m}, in order of
 *            increasing L. For a given n the returned value is the tuple...
 * Range:     u8 n <= 84, u16 n <= 3906, u32 n <= 3906, u64 n <= 3906,
 *            u128 n <= 3906, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* safety */

    seq_t cum = 0;                /* number of tuples processed so far */

    for (idx_t m = 1; ; ++m) {

        /* Number of new tuples generated when the maximum allowed entry
           becomes m.  For a fixed length L (1 <= L <= m) we add:
           - all tuples if m == L (this is the first time tuples of
             length L appear);
           - otherwise, only those tuples that contain at least one
             entry equal to m.  */
        seq_t block = 0;
        for (idx_t L = 1; L <= m; ++L) {
            seq_t term;
            if (m == L) {
                term = pow_seq((seq_t)m, (int)L);
            } else {
                term = pow_seq((seq_t)m, (int)L) -
                       pow_seq((seq_t)(m - 1), (int)L);
            }
            block += term;
        }

        if (cum + block >= n) {         /* the desired tuple is inside this block */
            seq_t offset = n - cum;     /* 1‑based offset inside the block */

            for (idx_t L = 1; L <= m; ++L) {
                seq_t term;
                if (m == L) {
                    term = pow_seq((seq_t)m, (int)L);
                } else {
                    term = pow_seq((seq_t)m, (int)L) -
                           pow_seq((seq_t)(m - 1), (int)L);
                }

                if (offset <= term)      /* the tuple lies in this L‑block */
                    return (seq_t)L;

                offset -= term;          /* skip this block */
            }
        }

        cum += block;                   /* move to the next maximum value */
    }
}
