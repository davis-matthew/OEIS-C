/*
 * A032921  Numbers whose set of base-14 digits is {1,3}.
 * https://oeis.org/A032921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer whose base‑14 representation uses only
 *            the digits 1 and 3 (increasing order)
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 766, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)       /* undefined; return 0 for robustness */
        return (seq_t)0;

    idx_t remaining = n;          /* numbers left to skip */
    idx_t count = 1;              /* 2^0 */
    unsigned int len = 0;         /* current length in base‑14 */

    /* Find the length len such that the n‑th number has that many digits. */
    while (1) {
        len += 1;
        count <<= 1;              /* count = 2^len */
        if (remaining <= count)
            break;
        remaining -= count;      /* skip all numbers of this length */
    }

    idx_t m = remaining - 1;      /* zero‑based index within length len */

    seq_t val = 0;
    for (unsigned int i = 0; i < len; ++i) {
        val = val * (seq_t)14;    /* shift left by one base‑14 digit */

        /* Decide whether this position is 1 or 3 */
        if ((m >> (len - 1 - i)) & (idx_t)1)
            val += (seq_t)3;
        else
            val += (seq_t)1;
    }

    return val;
}
