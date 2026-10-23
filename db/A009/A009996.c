/*
 * A009996  Numbers with digits in nonincreasing order.
 * https://oeis.org/A009996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A009996: numbers with decimal digits in nonincreasing order.
 * This implementation finds the number of digits by accumulating counts
 * of all sequences of nonincreasing digits and then builds the
 * required number by successive combinatorial rank reduction.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* n is 1-indexed; the first term (n==1) is 0. */
    seq_t n_seq = (seq_t)n;          /* use seq_t for all arithmetic */
    seq_t cumulative = 0;            /* total count of numbers with fewer digits */

    /* Find the length (number of digits) of a(n). */
    idx_t len = 0;                   /* number of digits */
    seq_t remaining = n_seq;         /* rank within the current digit-length block */
    while (1) {
        len++;
        seq_t count_len;
        if (len == 1) {              /* 1‑digit numbers: 0 … 9 */
            count_len = 10;
        } else {
            /* Count of nonincreasing sequences of length len with leading digit≠0 */
            count_len = binomial((idx_t)(len + 9), (idx_t)len) - 1;
        }
        if (remaining <= count_len) {
            break;                   /* a(n) has this number of digits */
        }
        remaining -= count_len;       /* skip this whole block */
    }

    /* Build the number digit by digit, starting from the most significant. */
    seq_t r = remaining;             /* rank within the block (1‑based) */
    seq_t result = 0;
    int max_allowed = 9;             /* maximum allowed digit for the next position */

    for (int pos = (int)len - 1; pos >= 0; --pos) {
        int min_digit = 0;
        if (pos == (int)len - 1 && len > 1) {
            min_digit = 1;           /* leading digit cannot be 0 (except for 1‑digit numbers) */
        }
        int chosen = -1;
        for (int d = min_digit; d <= max_allowed; ++d) {
            /* Number of ways to fill the remaining 'pos' lower digits
               when the current digit is 'd' (digits are ≤ d). */
            seq_t cnt = binomial((idx_t)(pos + d), (idx_t)pos);
            if (r > cnt) {
                r -= cnt;             /* skip these combinations */
            } else {
                chosen = d;
                max_allowed = d;     /* next lower digit cannot exceed this one */
                break;
            }
        }
        result = result * (seq_t)10 + (seq_t)chosen;
    }

    return result;
}
