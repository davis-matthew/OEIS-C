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
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

/* number of base‑8 digits of v (v >= 0) */
static inline int digit_len_base8(seq_t v)
{
    int l = 0;
    if (v == (seq_t)0) return 1;
    while (v) {
        v /= (seq_t)8;
        ++l;
    }
    return l;
}

/* integer base‑8 logarithm: floor(log_8(x))  (x >= 0) */
static inline idx_t log8_idx(idx_t x)
{
    idx_t r = 0;
    while (x >= 8) {
        x /= 8;
        ++r;
    }
    return r;
}

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
