/*
 * A043514  Numbers having two 6's in base 10.
 * https://oeis.org/A043514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer with exactly two digits 6 in
 *            base 10 * (sorted in increasing order, offset a(1)=66)
 * Range:     u8 n <= 2, u16 n <= 4224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses combinatorial counting to locate the n‑th number
 * without generating all preceding ones. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

static seq_t pow9(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 9;
    return r;
}

static seq_t count_len(idx_t len)
{
    if (len < 2)
        return 0;

    seq_t res = 0;
    /* first case: leading digit is 6 */
    if (len >= 2) {
        seq_t pow1 = pow9(len - 2);                /* 9^(len-2) */
        res += (seq_t)(len - 1) * pow1;
    }

    /* second case: leading digit not 6 */
    if (len >= 3) {
        seq_t pow2 = pow9(len - 3);                /* 9^(len-3) */
        seq_t comb = (seq_t)(len - 1) * (len - 2) / 2;   /* C(len-1,2) */
        res += comb * 8 * pow2;
    }

    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1, but harmless */

    /* Find the length L such that the n-th number lies among the L‑digit
     * numbers with exactly two 6's. */
    idx_t len = 2;
    seq_t cumulative = 0;
    seq_t target = (seq_t)n;
    while (cumulative + count_len(len) < target) {
        cumulative += count_len(len);
        ++len;
    }

    /* Position of the desired number among the len‑digit numbers (0‑based). */
    seq_t r = target - cumulative - 1;
    seq_t value = 0;
    idx_t remaining6 = 2;

    for (idx_t pos = 1; pos <= len; ++pos) {
        idx_t remPos = len - pos;
        int start_digit = (pos == 1) ? 1 : 0;

        for (int d = start_digit; d <= 9; ++d) {
            idx_t rem6 = remaining6;
            if (d == 6)
                --rem6;
            if ((idx_t)rem6 > remPos || (idx_t)rem6 < 0)
                continue;

            seq_t ways = binomial(remPos, rem6) * pow9(remPos - rem6);
            if (r >= ways) {
                r -= ways;
                continue;
            }

            value = value * 10 + (seq_t)d;
            remaining6 = rem6;
            break;
        }
    }

    return value;
}
