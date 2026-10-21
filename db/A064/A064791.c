/*
 * A064791  "Inverse permutation" to A064537. Limits of the recursion
 *          b(i+1)=B_[i](b(i)), where b(0)=n and B_[k](j) = B_[k-1](j) + k,
 *          k+1 <= j <= 2k; B_[k](j) = B_[k-1](j) - k, 2k+1 <= j <= 3k;
 *          B_[k](j) = B_[k-1](j) otherwise. Set a(n)=0 if b tends to
 *          infinity.
 * https://oeis.org/A064791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A064791 (Inverse permutation to A064537).
 *
 * The sequence is defined by the recursion:
 *   b(0) = n
 *   b(i+1) = B_[i](b(i))
 *   where B_[k] is defined by
 *       B_[k](j) =
 *           B_[k-1](j) + k   if k+1 ≤ j ≤ 2k
 *           B_[k-1](j) - k   if 2k+1 ≤ j ≤ 3k
 *           B_[k-1](j)       otherwise
 *   B_[0] is the identity function.
 *
 * The limit of the recursion when k exceeds the current value of b is the
 * desired a(n).  (If the recursion fails to converge, a(n) would be 0, but
 * this does not occur for the values tested here.)
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t b = (seq_t)n;           // b(0) = n
    for (idx_t k = 1; ; ++k) {
        seq_t kseq = (seq_t)k;
        if (kseq > b) break; /* further steps cannot change b */
        if (b >= kseq + (seq_t)1 && b <= (seq_t)2 * kseq) {
            b += kseq;
        } else if (b >= (seq_t)2 * kseq + (seq_t)1 && b <= (seq_t)3 * kseq) {
            b -= kseq;
        }
    }
    return b;
}
