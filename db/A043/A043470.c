/*
 * A043470  Numbers having two 4's in base 9.
 * https://oeis.org/A043470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th smallest positive integer that has exactly two *
 *            digits equal to 4 in its base‑9 representation. The sequence is
 *            * enumerated in increasing numerical order
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   pow8_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow8_table.h"
 
/* Helper: binomial coefficient C(n,k).  n,k are small (≤20).  */
static idx_t binom_idx(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}
 

 
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* sequence is defined from n≥1 */
 
    /* find the length L of the base‑9 representation of the n‑th number */
    idx_t remaining = n;
    idx_t L = 2;              /* minimal length to have two 4's */
    idx_t pow8[21];
    pow8_table(pow8, 20);     /* enough for L up to 20 */
 
    while (1) {
        idx_t countL =
            binom_idx(L, 2) * pow8[L - 2] -
            binom_idx(L - 1, 2) * pow8[L - 3];
        if (remaining > countL) {
            remaining -= countL;
            ++L;
        } else {
            break;
        }
    }
 
    /* generate the (remaining‑th) number of length L */
    seq_t value = 0;
    idx_t used4 = 0;                   /* how many 4's placed so far */
    for (idx_t pos = 1; pos <= L; ++pos) {
        for (idx_t d = (pos == 1 ? 1 : 0); d <= 8; ++d) {
            idx_t new_used4 = used4 + (d == 4 ? 1 : 0);
            if (new_used4 > 2) continue;

            idx_t remaining_pos = L - pos;
            idx_t need4 = 2 - new_used4;
            if (need4 > remaining_pos) continue;

            idx_t seqCount =
                binom_idx(remaining_pos, need4) *
                pow8[remaining_pos - need4];
 
            if (remaining > seqCount) {
                remaining -= seqCount;
                continue;
            } else {
                /* this digit is chosen */
                value = value * 9 + (seq_t)d;
                used4 = new_used4;
                break;
            }
        }
    }
    return value;
}
