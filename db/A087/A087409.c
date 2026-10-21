/*
 * A087409  Multiples of 6 with digits grouped in pairs and leading zeros
 *          omitted.
 * https://oeis.org/A087409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, look
 * Formula:   * a(n) is the n-th two‑digit block obtained by writing all
 *            positive multiples * of six consecutively in base‑10 and
 *            grouping this digit stream into pairs * (leading zeros omitted)
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"   /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h" /* int digit_len(seq_t x); */

static seq_t digit_at(seq_t x, idx_t pos, idx_t len)
{
    idx_t shift = len - 1 - pos;            /* position from left (0‑based) */
    seq_t div   = pow10(shift);
    return (x / div) % 10;
}

seq_t a(idx_t n)               /* n ≥ 1 */
{
    idx_t block_idx = n - 1;            /* zero‑based block index */
    idx_t target_pos = block_idx * 2;   /* zero‑based digit position of the block */

    idx_t pos = 0;                      /* digits processed so far */
    idx_t k = 1;                        /* multiple index (i.e., n in 6*n) */

    while (1) {
        seq_t cur = (seq_t)6 * (seq_t)k;     /* current multiple of 6 */
        idx_t len = (idx_t)digit_len(cur);   /* number of decimal digits */

        if (pos + len <= target_pos) {      /* block not in this number */
            pos += len;
            ++k;
            continue;
        }

        /* block starts inside cur */
        idx_t offset = target_pos - pos;     /* offset of first digit in cur */

        seq_t first_digit = digit_at(cur, offset, len);

        seq_t second_digit;
        if (offset + 1 < len) {
            second_digit = digit_at(cur, offset + 1, len);
        } else { /* second digit comes from next multiple */
            seq_t next_cur = (seq_t)6 * (seq_t)(k + 1);
            idx_t next_len = (idx_t)digit_len(next_cur);
            second_digit = digit_at(next_cur, 0, next_len);
        }

        return first_digit * 10 + second_digit;
    }
}
