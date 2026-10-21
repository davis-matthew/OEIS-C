/*
 * A085445  Numbers equal to exactly twice the sum of the products of each of
 *          their digits.
 * https://oeis.org/A085445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   The n‑th term is the n‑th integer N such that N = 2⋅S² with *
 *            digit_sum(N) = S. The sequence is finite; after the 5 known
 *            terms * it contains no further entries. The function returns 0
 *            for n larger * than the number of terms found
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* pre‑compute all solutions (they are only five) */
    static seq_t seq[200];
    static idx_t  len = 0;
    static int    done = 0;

    if (!done) {
        done = 1;
        const idx_t maxS = 2000;          /* safe upper bound; see analysis */
        for (idx_t s = 0; s <= maxS; ++s) {
            seq_t val = (seq_t)s;
            val = val * val;   /* val = s²   */
            val = val * 2;     /* val = 2⋅s² */

            /* compute digit sum of val */
            seq_t sum = 0;
            seq_t t = val;
            while (t > 0) {
                sum += t % (seq_t)10;
                t /= (seq_t)10;
            }

            if (sum == s) {
                if (len < sizeof(seq)/sizeof(seq[0]))
                    seq[len++] = val;
            }
        }
    }

    if (n == 0)          /* offset is 1; undefined for n<1 */
        return 0;
    if (n <= len)
        return seq[n - 1];
    return 0;            /* no further terms */
}
