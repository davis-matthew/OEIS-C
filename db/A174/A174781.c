/*
 * A174781  Duplicate of A028846.
 * https://oeis.org/A174781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 27, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n‑th positive integer whose decimal digits belong to {1,2,4,8} (offset 1) */
seq_t a(idx_t n)
{
    static const int digit[4] = {1, 2, 4, 8};
    idx_t len   = 1;   /* current number of decimal digits */
    idx_t count = 4;   /* 4^len: terms with exactly len digits */
    idx_t cum   = 0;   /* terms with fewer than len digits */

    /* Find the length of the n‑th term */
    while (n - cum > count) {
        cum   += count;   /* add all shorter blocks */
        len++;            /* move to next length */
        count *= 4;       /* 4^(len) */
    }

    /* Position within the block (0‑based) */
    idx_t pos = n - cum - 1;

    /* Extract base‑4 digits of pos, padded to length len */
    int digits[64];
    for (idx_t i = 0; i < len; ++i) {
        digits[len - 1 - i] = pos % 4;
        pos /= 4;
    }

    /* Convert to decimal number with digits 1,2,4,8 */
    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i) {
        result = result * 10 + digit[digits[i]];
    }
    return result;
}
