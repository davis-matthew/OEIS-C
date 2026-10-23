/*
 * A045193  Numbers whose base-5 representation contains exactly one 0 and two
 *          3's.
 * https://oeis.org/A045193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base-5 representation contains exactly one
 *            0 and two 3's
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1u) return (seq_t)0;
    /* Find the length (number of base-5 digits) of the n-th term */
    idx_t remaining = n;
    idx_t len = 3; /* minimal length is 3 (one 0 and two 3's) */

    while (1) {
        /* count = (len-1) * C(len-1,2) * 3^(len-3) */
        idx_t comb = (len - 1u) * (len - 2u) / 2u; /* C(len-1,2) */
        idx_t pow3 = 1u;
        for (idx_t i = 0u; i < len - 3u; ++i) pow3 *= 3u;
        idx_t count = (len - 1u) * comb * pow3;
        if (remaining <= count) break;
        remaining -= count;
        ++len;
    }

    /* Enumerate numbers of this length until we find the remaining-th one */
    idx_t start = 1u;
    for (idx_t i = 1u; i < len; ++i) start *= 5u;    /* 5^(len-1) */
    idx_t end   = start * 5u;                         /* 5^len */
    idx_t offset = remaining;                         /* 1‑based within this length */

    for (idx_t val = start; val < end; ++val) {
        idx_t copy = val;
        idx_t cnt0 = 0u;
        idx_t cnt3 = 0u;
        for (idx_t i = 0u; i < len; ++i) {
            idx_t digit = copy % 5u;
            copy /= 5u;
            if (digit == 0u) ++cnt0;
            else if (digit == 3u) ++cnt3;
        }
        if (cnt0 == 1u && cnt3 == 2u) {
            --offset;
            if (offset == 0u) return (seq_t)val;
        }
    }
    return (seq_t)0; /* should never be reached */
}
