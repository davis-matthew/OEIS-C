/*
 * A008014  Coordination sequence T1 for Zeolite Code AFI.
 * https://oeis.org/A008014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n >= 10, a(n) = 210*m^2 + 42*k*m + c_k, where m = n / 10, k
 *            = n % 10, and c_k = {2,4,11,21,35,53,77,105,137,172}. For 0 <= n
 *            < 10, a(n) equals {1,4,11,21,35,53,77,105,137,172} respectively
 * Range:     u8 n <= 10, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


static const seq_t small_tbl[10] = {1,4,11,21,35,53,77,105,137,172};
static const seq_t coeff_tbl[10] = {2,4,11,21,35,53,77,105,137,172};

seq_t a(idx_t n) {
    if (n < 10) {
        return small_tbl[(size_t)n];
    } else {
        idx_t m = n / 10;
        idx_t k = n % 10;
        seq_t mm = (seq_t)m;
        seq_t term = (seq_t)210 * mm * mm +
                     (seq_t)42 * ((seq_t)k) * mm +
                     coeff_tbl[(size_t)k];
        return term;
    }
}
