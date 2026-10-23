/*
 * A049982  Number of arithmetic progressions of 2 or more positive integers,
 *          strictly increasing with sum n.
 * https://oeis.org/A049982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arithmetic progressions of length ≥2 with
 *            strictly increasing positive terms summing to n. For each
 *            possible length k (2 ≤ k ≤ T_k ≤ n): Let A = k(k−1)/2. Let d_max
 *            = ⌊(n−k)/A⌋ (maximum common difference giving a ≥1). For k odd:
 *            a progression exists iff n ≡ 0 (mod k); then all d...
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t S = (seq_t)n;          /* convert index to sequence type */
    seq_t total = 0;
    for (idx_t k = 2; ; ++k) {
        /* triangular number T_k = k(k+1)/2 */
        seq_t tri = ( (seq_t)k * (k + 1) ) / 2;
        if (tri > S) break;       /* no further lengths possible */

        seq_t A = ( (seq_t)k * (k - 1) ) / 2;   /* k(k-1)/2 */
        if (A == 0) continue;   /* safety for k=1, though loop starts at 2 */

        seq_t dmax = (S - (seq_t)k) / A;
        if (dmax < 1) continue;

        if (k & 1ULL) {   /* odd length */
            if (S % (seq_t)k == 0) {
                total += dmax;
            }
        } else {          /* even length */
            seq_t r = S % (seq_t)k;
            if (r == 0) {
                total += dmax / 2;
            } else if (r == ((seq_t)k / 2)) {
                total += (dmax + 1) / 2;
            }
        }
    }
    return total;
}
