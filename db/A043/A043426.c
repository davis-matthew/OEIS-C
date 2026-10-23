/*
 * A043426  Numbers having two 1's in base 8.
 * https://oeis.org/A043426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute A043426: Numbers having two 1's in base 8.
 * The routine works by first finding how many digits the n‑th number
 * has, then scanning all numbers with that number of octal digits and
 * counting those whose base‑8 representation contains exactly two digits
 * equal to 1.
 *
 * Note: this is a direct search algorithm; it does not use a closed
 * formula but it is efficient enough for the required limits.
 */

#include <stdint.h>   /* for uint8_t */

typedef idx_t idx_t;   /* provided by the build */
typedef seq_t seq_t;   /* provided by the build */

/* Count how many d‑digit octal numbers have exactly two ‘1’ digits.
 * For d < 2 the count is 0, for d == 2 it is 1 (only 11₈ = 9).  */
static seq_t count_len(idx_t d)
{
    if (d < 2)
        return 0;
    if (d == 2)
        return 1;

    /* compute 7^(d-3) */
    seq_t pow7 = 1;
    for (idx_t i = 0; i < d - 3; ++i)
        pow7 *= 7;

    /* formula: (d-1) * (3*d + 1) * 7^(d-3) */
    seq_t term = (seq_t)(d - 1) * ((seq_t)3 * d + 1) * pow7;
    return term;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset not used; defined for completeness */

    /* 1. Find the digit length d of the n‑th term */
    idx_t d = 1;
    seq_t cumulative = 0;
    for (;; ++d) {
        seq_t cnt = count_len(d);
        if (cumulative + cnt >= n)
            break;
        cumulative += cnt;
    }

    /* 2. Determine the zero‑based offset within this d‑digit block */
    seq_t offset = (seq_t)n - cumulative - 1;   /* 0‑based index */

    /* 3. Compute the range of decimal values that have d octal digits
       (from 8^(d-1) to 8^d - 1) */
    seq_t start = 1;
    for (idx_t i = 1; i < d; ++i)
        start *= 8;

    seq_t end = start * 8 - 1;

    /* 4. Scan through the range, counting numbers with exactly two
       octal digits equal to 1.  When the offset reaches zero, we
       have found the desired value. */
    for (seq_t val = start; val <= end; ++val) {
        seq_t tmp = val;
        idx_t ones = 0;

        while (tmp != 0) {
            uint8_t digit = (uint8_t)(tmp % 8);
            if (digit == 1)
                ++ones;
            if (ones > 2)
                break;
            tmp /= 8;
        }

        if (ones == 2) {
            if (offset == 0)
                return val;
            --offset;
        }
    }

    /* Should never reach here for valid n */
    return 0;
}
