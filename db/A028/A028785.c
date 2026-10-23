/*
 * A028785  Nonsquares mod 72.
 * https://oeis.org/A028785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth positive integer that is not a quadratic residue modulo 72 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* one-time computation of the 72 residues that are quadratic
       residues modulo 72, and the complementary list of
       nonsquare residues.  The pattern repeats every 72 integers. */
    static int initialized = 0;
    static unsigned char isSq[72];
    static unsigned char nonsq[72];
    static size_t nonsq_cnt;

    if (!initialized) {
        /* mark all residues that are quadratic residues modulo 72 */
        for (int i = 0; i < 72; ++i) isSq[i] = 0;
        for (int x = 0; x < 72; ++x) {
            int r = (x * x) % 72;
            isSq[r] = 1;
        }
        /* collect all residues 1 .. 72 that are not quadratic residues */
        nonsq_cnt = 0;
        for (int r = 1; r <= 72; ++r) {
            if (!isSq[r % 72])
                nonsq[nonsq_cnt++] = (unsigned char)r;
        }
        initialized = 1;
    }

    /* The sequence is periodic with period 72, containing
       "nonsq_cnt" entries per period. */
    idx_t block = (n - 1) / nonsq_cnt;           /* which period */
    idx_t pos   = (n - 1) % nonsq_cnt;           /* index inside period */

    /* combine the period number with the residue to get the value */
    return (seq_t)((seq_t)block * 72 + (seq_t)nonsq[(size_t)pos]);
}
