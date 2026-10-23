/*
 * A023750  Plaindromes: numbers whose digits in base 8 are in nondecreasing
 *          order.
 * https://oeis.org/A023750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 1254, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Helpers:   binom_idx_t, comb128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth number (with a(1)=0) whose base‑8 digits are
 * non‑decreasing (from most significant to least). The implementation
 * computes the value by combinatorial ranking of weakly increasing digit
 * sequences.
 */

#include <stdint.h>
#include "../utils/binom_idx_t.h"
#include "../utils/comb128.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n == 1) return (seq_t)0;

    /* Rank among non‑zero terms (k = 1 means the first non‑zero term). */
    idx_t k = n - 1;          /* 1‑based rank of the desired non‑zero number */

    /* Find the length of the base‑8 representation. */
    idx_t cum = 0;
    unsigned len = 0;
    while (1) {
        ++len;                               /* length >= 1 */
        idx_t cnt = binom_idx_t(len + 6);     /* C(len+6,6) */
        if (cum + cnt >= k) break;           /* target length found */
        cum += cnt;
    }

    /* 0‑based rank within sequences of this length. */
    idx_t rank = k - cum - 1;                /* 0 <= rank < C(len+6,6) */

    /* Unrank to get the actual digits. */
    unsigned prev_digit = 1;                 /* first digit cannot be 0 */
    seq_t value = 0;                         /* decimal value */

    for (unsigned pos = 0; pos < len; ++pos) {
        unsigned remaining = len - pos - 1;  /* digits left after current */
        unsigned chosen_digit = 0;

        for (unsigned d = prev_digit; d <= 7; ++d) {
            /* Count how many sequences start with digit d at this position. */
            unsigned types = 8 - d;          /* available digits >= d */
            unsigned nC = types + remaining - 1;
            unsigned kC = remaining;
            __uint128_t cnt128 = comb128(nC, kC);
            if ((__uint128_t)rank < cnt128) {
                chosen_digit = d;
                break;
            } else {
                rank -= (idx_t)cnt128;  /* safe: cnt fits in idx_t for our ranges */
            }
        }

        /* Append chosen_digit to the result. */
        value = value * (seq_t)8 + (seq_t)chosen_digit;
        prev_digit = chosen_digit;
    }

    return value;
}
