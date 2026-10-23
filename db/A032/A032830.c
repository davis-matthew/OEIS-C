/*
 * A032830  Numbers whose set of base-6 digits is {3,4}.
 * https://oeis.org/A032830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base‑6 representation *
 *            uses only the digits 3 and 4. The algorithm finds the length of
 *            the * base‑6 representation by subtracting the counts of shorter
 *            lengths, * then interprets the offset within that length as a
 *            binary number, * mapping 0→3 and 1→4
 * Range:     u8 n <= 14, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; 0 is unused but safe */

    idx_t remaining = n;
    idx_t len = 0;                 /* number of base‑6 digits */

    /* Determine the digit length that contains the n-th term */
    while (1) {
        ++len;
        idx_t count_len = ((idx_t)1) << len;   /* 2^len */
        if (remaining <= count_len)
            break;
        remaining -= count_len;
    }

    /* remaining is between 1 and 2^len, inclusive */
    idx_t k = remaining - 1;       /* 0‑based offset within this length */

    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i) {
        unsigned bit = (k >> (len - 1 - i)) & 1ULL;
        val = val * 6 + (seq_t)(3 + bit);   /* map 0→3, 1→4 */
    }

    return val;
}
