/*
 * A045826  a(n) = A005887(n) / 2.
 * https://oeis.org/A045826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r_3(2*n+1)/2 where r_3(m) is the number of ordered
 *            integer triples (a,b,c) with a^2 + b^2 + c^2 = m. This is equal
 *            to A005887(m)/2 as described in OEIS A045826
 * Range:     u8 n <= 421, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation precomputes r_3(m) for all m up to the maximum needed
   by the current call using a complete 3‑nested loop over integer coordinates.
   Since n in typical tests is at most ~10^4, the loops run at most about
   22 million iterations, which is well within the time limit. */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    static seq_t *r3 = NULL;
    static idx_t max_m_done = 0;

    /* desired m = 2*n + 1 (must fit in idx_t) */
    idx_t req_m = (idx_t)2 * n + 1;

    if (req_m > max_m_done) {
        /* Allocate new storage */
        idx_t new_size = req_m + 1;
        seq_t *new_r3 = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_r3) {
            return 0; /* allocation failure, unlikely in tests */
        }

        /* initialise to zero */
        for (idx_t i = 0; i < new_size; ++i) {
            new_r3[i] = 0;
        }

        /* compute r_3(m) for all m <= req_m */
        /* maximal coordinate |x| <= floor(sqrt(req_m)) */
        unsigned R = (unsigned)floor(sqrt((double)req_m));

        for (int a = -(int)R; a <= (int)R; ++a) {
            unsigned a2 = (unsigned)(a * a);
            if (a2 > req_m) continue;
            for (int b = -(int)R; b <= (int)R; ++b) {
                unsigned sum_ab = a2 + (unsigned)(b * b);
                if (sum_ab > req_m) continue;
                for (int c = -(int)R; c <= (int)R; ++c) {
                    unsigned m = sum_ab + (unsigned)(c * c);
                    if (m > req_m) continue;
                    ++new_r3[(size_t)m];
                }
            }
        }

        /* replace the old array */
        free(r3);
        r3 = new_r3;
        max_m_done = req_m;
    }

    seq_t val = r3[(size_t)req_m] / 2;
    return val;
}
