/*
 * A178801  Write n! partition(n) times.
 * https://oeis.org/A178801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabf, nonn
 * Formula:   a(n) = (k)! where k is the smallest integer such that *
 *            Σ_{i=0}^{k} partition(i) > n. * Here partition(i) is the
 *            partition function A000041
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static arrays for cached partitions, cumulative sums and factorials */
    static idx_t  *part = NULL, *cum = NULL;
    static seq_t *fact = NULL;
    static size_t sz   = 0, cap = 0;

    /* initial allocation */
    if (cap == 0) {
        cap = 16;
        part = (idx_t*)malloc(cap * sizeof(idx_t));
        cum  = (idx_t*)malloc(cap * sizeof(idx_t));
        fact = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!part || !cum || !fact) return (seq_t)0; /* unlikely error */

        part[0] = (idx_t)1;   /* partition(0) = 1 */
        cum[0]  = (idx_t)1;   /* sum to 0 is 1 */
        fact[0] = (seq_t)1;   /* 0! = 1 */
        sz = 1;
    }

    /* extend arrays until cumulative sum exceeds n */
    while (cum[sz - 1] <= n) {
        idx_t i = sz;  /* compute partition(i) */

        /* Pentagonal number theorem for partition computation */
        int64_t sum = 0;
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent1 > i && pent2 > i) break;

            int64_t term = 0;
            if (pent1 <= i) term += (int64_t)part[i - pent1];
            if (pent2 <= i) term += (int64_t)part[i - pent2];
            if (k & 1)
                sum += term;
            else
                sum -= term;
        }
        part[i] = (idx_t)sum;

        cum[i] = cum[i - 1] + part[i];

        fact[i] = fact[i - 1] * (seq_t)i;

        ++sz;

        /* resize if necessary */
        if (sz == cap) {
            cap <<= 1;
            part = (idx_t*)realloc(part, cap * sizeof(idx_t));
            cum  = (idx_t*)realloc(cum,  cap * sizeof(idx_t));
            fact = (seq_t*)realloc(fact, cap * sizeof(seq_t));
        }
    }

    return fact[sz - 1];
}
