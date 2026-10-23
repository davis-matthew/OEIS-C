/*
 * A187769  Triangle read by rows: equivalence classes of natural numbers,
 *          where numbers are equivalent when having equal numbers of zeros
 *          and ones in binary representation, respectively.
 * https://oeis.org/A187769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) is the permutation of the natural numbers obtained by
 *            grouping the numbers of equal binary length, ranking each group
 *            first by increasing popcount (number of ones) and, within equal
 *            popcount, by ascending numeric value
 * Range:     u8 n <= 255, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Local helper: compute binomial coefficients C(n,k) for 0 <= k <= n <= 63.
   The values fit in 64 bits for all such inputs. */
static uint64_t binom_uint64(uint64_t n, uint64_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* The first term a(0) is defined as 0. */
    if (n == 0) return 0;

    /* Determine the binary‑length row r such that 2^r <= n < 2^(r+1).
       For n >= 1, r = floor(log2(n)). */
    int r = 63 - __builtin_clzll((unsigned long long)n);

    /* The first index of this row in the flattened sequence. */
    idx_t start = (idx_t)1 << r;

    /* Position of the desired element inside its row (0‑based). */
    idx_t offset = n - start;

    /* Find the popcount group t (number of ones among the lower r bits)
       that contains this offset. */
    int t = 0;                 /* popcount of lower bits */
    idx_t rem = offset;        /* remaining offset inside the group */

    while (t <= r) {
        idx_t cnt = (idx_t)binom_uint64(r, t);  /* C(r, t) */
        if (rem < cnt) break;                  /* the desired element is here */
        rem -= cnt;
        ++t;
    }

    /* Unrank rem within the group of numbers having exactly t ones
       among r lower bits.  This builds the lower part of the number. */
    seq_t lower = 0;
    int ones = t;
    idx_t idx = rem;

    for (int pos = r - 1; pos >= 0 && ones > 0; --pos) {
        uint64_t cnt0 = binom_uint64(pos, ones);   /* combinations if this bit is 0 */
        if (idx >= cnt0) {                         /* the bit must be 1 */
            lower |= (seq_t)((uint64_t)1 << pos);
            idx -= cnt0;
            --ones;
        }
        /* else the bit stays 0; nothing to do */
    }

    /* Assemble the full value: the leading 1 at bit r plus the lower bits. */
    seq_t high = (seq_t)((uint64_t)1 << r);
    return high | lower;
}
