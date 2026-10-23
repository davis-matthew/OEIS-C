/*
 * A023810  Xenodromes: all digits in base 16 are different.
 * https://oeis.org/A023810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   a(n) = the n‑th smallest non‑negative integer whose *
 *            hexadecimal representation contains no repeated digits * (digits
 *            0–9, A–F). The sequence is enumerated in * increasing decimal
 *            order starting with a(1)=0
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* compute P(n,r) = n! / (n-r)! for 0 <= r <= n <= 16
 * result fits into 64 bits (n! <= 16! = 20922789888000)   */
static inline uint64_t perm(uint8_t n, uint8_t r)
{
    uint64_t res = 1;
    for (uint8_t i = 0; i < r; ++i)
        res *= (uint64_t)(n - i);
    return res;
}

/* a(n) with 1‑based index */
seq_t a(idx_t n)
{
    /* first 16 numbers are 0 through 15 */
    if (n <= 16)
        return (seq_t)(n - 1);

    /* n >= 17: work with numbers having at least two hex digits */
    idx_t remaining = n - 16;          /* 1‑based offset within the tail */

    /* determine the length of the hex representation (2–16 ) */
    int len = 2;
    for (;; ++len) {
        uint64_t count_len = 15ULL * perm(15, len - 1); /* 15 first digit choices */
        if (remaining <= count_len)
            break;
        remaining -= count_len;
    }

    /* construct the number digit by digit */
    seq_t result = 0;
    unsigned int used = 0;  /* bit mask of digits already used */

    for (int pos = 0; pos < len; ++pos) {
        int start_digit = (pos == 0) ? 1 : 0;          /* leading digit cannot be zero */
        for (int d = start_digit; d < 16; ++d) {
            if (used & (1u << d))
                continue;                            /* already used */

            /* number of ways to fill the remaining positions */
            uint8_t avail = 16u - (pos + 1);           /* digits still unused */
            uint8_t rem_pos = len - pos - 1;
            uint64_t cnt = perm(avail, rem_pos);

            if (remaining > cnt) {
                remaining -= cnt;                     /* skip this block */
                continue;
            }

            /* choose this digit */
            result <<= 4;
            result |= (seq_t)d;
            used |= (1u << d);
            break;
        }
    }

    return result;
}
