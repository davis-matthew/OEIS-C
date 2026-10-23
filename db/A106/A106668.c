/*
 * A106668  Marks on lexicographically second 6-mark optimal Golomb ruler.
 * https://oeis.org/A106668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the marks of the lexicographically second optimal 6‑mark Golomb
   ruler.  The sequence a(n) is defined for n = 0 … 5.  The ruler has
   length 17 and the marks are [0,1,4,10,15,17].  The marks are found
   by an exhaustive search for a 6‑mark Golomb ruler of minimal
   length.  The search stops as soon as the lexicographically second
   solution is discovered. */

#include <stdbool.h>

static seq_t second_ruler[6];
static bool initialized = false;

static void compute_second_ruler(void)
{
    /* Search for the minimal length L of an optimal 6‑mark Golomb ruler. */
    for (idx_t len = 5; len <= 30; ++len) {         /* 30 is a safe upper bound */
        idx_t found = 0;
        seq_t cand[6];
        bool diff[256];

        /* Enumerate all increasing 5‑element combinations from 1..len */
        for (idx_t i1 = 1; i1 <= len - 4; ++i1) {
            for (idx_t i2 = i1 + 1; i2 <= len - 3; ++i2) {
                for (idx_t i3 = i2 + 1; i3 <= len - 2; ++i3) {
                    for (idx_t i4 = i3 + 1; i4 <= len - 1; ++i4) {
                        for (idx_t i5 = i4 + 1; i5 <= len; ++i5) {
                            /* The 6 marks of the candidate ruler */
                            cand[0] = 0;
                            cand[1] = (seq_t)i1;
                            cand[2] = (seq_t)i2;
                            cand[3] = (seq_t)i3;
                            cand[4] = (seq_t)i4;
                            cand[5] = (seq_t)i5;

                            /* Check the Golomb property: all differences distinct */
                            for (int d = 0; d < 256; ++d) diff[d] = false;
                            bool ok = true;
                            for (idx_t a = 0; a < 6 && ok; ++a) {
                                for (idx_t b = a + 1; b < 6; ++b) {
                                    idx_t d_val = (idx_t)(cand[b] - cand[a]); /* positive */
                                    if (diff[d_val]) { ok = false; break; }
                                    diff[d_val] = true;
                                }
                            }
                            if (!ok) continue;               /* not a Golomb ruler */

                            /* We found a solution of length len */
                            ++found;
                            if (found == 2) {                 /* lexicographically second */
                                for (int k = 0; k < 6; ++k)
                                    second_ruler[k] = cand[k];
                                initialized = true;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized)
        compute_second_ruler();

    if (n < 6)
        return second_ruler[n];
    return (seq_t)0;          /* undefined for n ≥ 6 */
}
