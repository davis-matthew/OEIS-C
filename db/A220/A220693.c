/*
 * A220693  Irregular triangle where the k-th item in the n-th row (both
 *          starting from 1) tells in how many ways we can add 2 distinct
 *          integers from 1 to n in such a way that the sum is divisible by k.
 *          Row n has 2n-1 terms.
 * https://oeis.org/A220693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   * Let r = floor(sqrt(n-1)) + 1 and k = n - (r-1)^2. * a(n) is
 *            the number of unordered distinct pairs {i,j} with * 1 ≤ i < j ≤
 *            r such that (i + j) mod k == 0. * For each residue s modulo k,
 *            let * cnt[0] = r / k * cnt[s] = (r + k - s) / k (1 ≤ s ≤ k-1) *
 *            Then * a(n) = C(cnt[0],2) * + (k even ? C(cn...
 * Range:     u8 n <= 529, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* integer square root: floor(sqrt(x)) for 0 ≤ x < 2^64 */
static inline idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0;
    idx_t hi = 1ULL << 32;          /* 2^32 is enough for 64‑bit sqrt */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid && mid > x / mid)   /* avoid overflow: mid*mid > x */
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return hi;
}

seq_t a(idx_t N) {
    if (N == 0) return 0;            /* sequence starts at n = 1 */

    /* Determine the row r and the position k within that row. */
    idx_t r = isqrt_u64(N - 1) + 1;   /* r ≥ 1 */
    idx_t pos = N - (r - 1) * (r - 1);/* 1 ≤ pos ≤ 2r-1 */
    idx_t k = pos;

    seq_t n = (seq_t)r;
    seq_t kseq = (seq_t)k;
    seq_t ans = 0;

    /* Pairs inside residue 0. */
    seq_t cnt0 = n / kseq;
    if (cnt0 > 1)
        ans += cnt0 * (cnt0 - 1) / 2;

    /* If k is even, pairs inside residue k/2. */
    if ((k & 1) == 0) {
        seq_t half = k / 2;
        seq_t cntHalf = (n + half) / kseq;   /* since k - half == half */
        if (cntHalf > 1)
            ans += cntHalf * (cntHalf - 1) / 2;
    }

    /* Pairs between different residues a and k-a (a < k-a). */
    idx_t limit = (k - 1) / 2;
    for (idx_t a = 1; a <= limit; ++a) {
        seq_t cntA = (n + kseq - (seq_t)a) / kseq;   /* residue a   */
        seq_t cntB = (n + (seq_t)a) / kseq;          /* residue k-a */
        ans += cntA * cntB;
    }

    return ans;
}
