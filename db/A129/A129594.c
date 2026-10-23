/*
 * A129594  Involution of nonnegative integers induced by the conjugation of
 *          the partition encoded in the run lengths of binary expansion of n.
 * https://oeis.org/A129594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): involution induced by conjugating the partition formed from
 * the run‑length composition of the binary expansion of n.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Step 1 : binary → composition (run lengths). */
    idx_t comp1[128];
    size_t runCnt = 0;
    idx_t temp = n;
    while (temp != 0) {
        idx_t bit = temp & 1;
        idx_t len = 0;
        while (temp != 0 && ((temp & 1) == bit)) {
            ++len;
            temp >>= 1;
        }
        comp1[runCnt++] = len;
    }

    /* Step 2 : composition → partition λ. */
    seq_t lambda[128];
    size_t k = runCnt;
    if (k == 0)
        return (seq_t)0;          /* should not happen for n > 0 */

    seq_t sum = (seq_t)comp1[0];
    lambda[0] = sum;
    for (size_t i = 1; i < k; ++i) {
        sum += (seq_t)(comp1[i] - 1);
        lambda[i] = sum;
    }

    /* Step 3 : conjugate λ to obtain partition μ. */
    idx_t mu[128];
    size_t muCnt = 0;
    if (k > 0) {
        seq_t maxLambda = lambda[k - 1];
        for (seq_t j = 1; j <= maxLambda; ++j) {
            size_t cnt = 0;
            for (size_t p = 0; p < k; ++p)
                if (lambda[p] >= j)
                    ++cnt;
            if (cnt == 0)
                break;
            mu[muCnt++] = (idx_t)cnt;
        }
    }

    /* Step 4 : μ → composition c′. */
    /* sort μ ascending. */
    idx_t muSorted[128];
    for (size_t i = 0; i < muCnt; ++i)
        muSorted[i] = mu[i];
    for (size_t i = 1; i < muCnt; ++i) {
        idx_t key = muSorted[i];
        size_t j = i;
        while (j > 0 && muSorted[j - 1] > key) {
            muSorted[j] = muSorted[j - 1];
            --j;
        }
        muSorted[j] = key;
    }

    idx_t comp2[128];
    size_t comp2Cnt = 0;
    if (muCnt > 0) {
        comp2[comp2Cnt++] = muSorted[0];
        for (size_t i = 1; i < muCnt; ++i) {
            idx_t diff = muSorted[i] - muSorted[i - 1];
            comp2[comp2Cnt++] = diff + 1;
        }
    }

    /* Step 5 : composition ⇢ integer. */
    seq_t val = 0;
    idx_t curBit = 1;              /* most significant bit is always 1 */
    for (int idx = (int)comp2Cnt - 1; idx >= 0; --idx) {
        idx_t len = comp2[idx];
        for (idx_t t = 0; t < len; ++t) {
            val = (val << 1) | curBit;
        }
        curBit ^= 1;               /* toggle the bit value */
    }
    return val;
}
