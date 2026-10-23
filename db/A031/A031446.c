/*
 * A031446  Numbers whose base-2 representation has 3 more 0's than 1's.
 * https://oeis.org/A031446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers whose binary representation has 3 more zeros than ones.
   This implementation uses combinatorial enumeration by bit length and
   combinadics to compute the nth term directly. */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) for small n (n ≤ 16). */
static inline unsigned long long C(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned long long res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (unsigned long long)(n - k + i) / (unsigned long long)i;
    }
    return res;
}

/* External linkage as required. */
seq_t a(idx_t n) {
    /* n is 1-based. Convert to 0-based index. */
    unsigned long long idx = (unsigned long long)n - 1ULL;

    /* Find the bit length L (odd, ≥5) such that the cumulative count
       of terms up to L exceeds idx. */
    int L = 5;
    unsigned long long cumulative = 0ULL;

    while (1) {
        int k = (L - 5) / 2;                   /* extra ones among lower bits */
        unsigned long long cnt = C(L - 1, k);   /* numbers of length L */

        if (cumulative + cnt > idx) break;     /* L is found */
        cumulative += cnt;
        L += 2;                                 /* next odd length */
    }

    /* Rank r of the desired number within all numbers of length L. */
    unsigned long long r = idx - cumulative;
    int k = (L - 5) / 2;          /* number of ones to place among lower bits */

    /* Build the (L-1)-bit mask with k ones using combinadics. */
    unsigned long long mask = 0ULL;
    unsigned long long rem = k;

    for (int pos = L - 2; pos >= 0 && rem > 0; --pos) {
        unsigned long long cnt0 = C(pos, rem);   /* combos if bit at pos is 0 */
        if (cnt0 <= r) {
            mask |= ((unsigned long long)1 << pos);
            r -= cnt0;
            --rem;
        }
    }

    /* Construct the final value: leading 1 followed by the mask. */
    unsigned long long val = ((unsigned long long)1 << (L - 1)) | mask;
    return (seq_t)val;
}
