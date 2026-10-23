/*
 * A181521  Representation of n = sum_k b_k*(k!!) in the double-factorial base
 *          by some b_k-fold concatenation of the indices k.
 * https://oeis.org/A181521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A181521: greedy decomposition in the double‑factorial base,
   followed by concatenation of the indices as a decimal number.   */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Work in seq_t to keep the precision adequate. */
    seq_t N = (seq_t)n;

    /* Pre‑compute double‑factorials up to the largest that does not exceed N. */
    seq_t dblf[64] = {0};
    dblf[1] = 1;                    /* 1!! */
    size_t max_k = 1;
    if (N >= 2) {
        dblf[2] = 2;                /* 2!! */
        max_k = 2;
    }
    for (size_t k = 3; k < 64; ++k) {
        /* Prevent overflow: stop if dblf[k-2] > N / k. */
        if (dblf[k-2] > 0 && dblf[k-2] > N / (seq_t)k)
            break;
        seq_t cur = (seq_t)k * dblf[k-2];
        if (cur > N)
            break;
        dblf[k] = cur;
        max_k = k;
    }

    /* Greedy decomposition: determine counts b[k] of each k!! used. */
    unsigned long long b[64] = {0};
    seq_t rem = N;
    for (size_t k = max_k; k >= 1; --k) {
        seq_t fact = dblf[k];
        seq_t use = rem / fact;           /* largest multiple fitting in rem */
        if (use) {
            b[k] = (unsigned long long)use;
            rem -= use * fact;
        }
        if (k == 1)                    /* avoid size_t underflow */
            break;
    }

    /* Build the decimal concatenation of indices. */
    seq_t result = 0;
    for (size_t k = 1; k <= max_k; ++k) {
        unsigned long long cnt = b[k];
        if (cnt == 0)
            continue;

        /* Number of decimal digits in k. */
        int len = 0;
        int tmp = (int)k;
        do {
            ++len;
            tmp /= 10;
        } while (tmp);

        /* Prepare 10^len. */
        seq_t ten_pow = 1;
        for (int i = 0; i < len; ++i)
            ten_pow *= 10;

        seq_t valk = (seq_t)k;
        for (unsigned long long i = 0; i < cnt; ++i)
            result = result * ten_pow + valk;
    }

    return result;
}
