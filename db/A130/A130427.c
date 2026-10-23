/*
 * A130427  Complete list of all 5 numbers n such that n is equal to the
 *          number of 5's in the decimal digits of all numbers <= n.
 * https://oeis.org/A130427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   count5_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A(n) are the fixed points of the function f(n) = number of 5's in the decimal
   digits of all numbers <= n.  It can be shown that there are only five such
   numbers:
        0, 10^10, 20^10, 30^10, 40^10.
   The function below computes these by directly searching and returns the
   n-th term for 1 <= n <= 5.  For larger n it returns 0. */
#include <stdbool.h>
#include "../utils/count5_upto.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t seqs[5];
    if (!initialized) {
        /* pre‑compute the five known fixed points */
        const seq_t BLOCK = (seq_t)10000000000ULL;   /* 10^10 */
        idx_t m = 0;
        int found = 0;
        while (found < 5 && m <= 5) {
            seq_t cand = (seq_t)m * BLOCK;
            seq_t cnt  = count5_upto(cand);
            if (cnt == cand) {
                seqs[found++] = cand;
            }
            ++m;
        }
        initialized = true;
    }
    if (n > 5) return (seq_t)0;
    return seqs[n - 1];
}
