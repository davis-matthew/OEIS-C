/*
 * A028429  Clog sequence in base 8. Right to left concatenation of n,
 *          int(log_8(n)),int(log_8(int(log_8(n)))),... in base8.
 * https://oeis.org/A028429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = right‑to‑left concatenation of n, floor(log_8(n)),
 *            floor(log_8(floor(log_8(n)))), …, interpreted as a base‑8 number
 *            and returned in decimal (i.e. as its integer value)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   digit_len_base8, log8_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */
#include "../utils/digit_len_base8.h"
#include "../utils/log8_idx.h"

seq_t a(idx_t n)
{
    /* start with the value of n itself */
    seq_t result = (seq_t)n;
    int len = digit_len_base8(result);

    idx_t logval = log8_idx(n);
    while (logval > 0) {
        /* shift = 8^len */
        seq_t shift = (seq_t)1;
        for (int i = 0; i < len; ++i)
            shift *= (seq_t)8;

        seq_t part = (seq_t)logval;
        result = part * shift + result;

        len += digit_len_base8(part);
        logval = log8_idx(logval);
    }

    return result;
}
