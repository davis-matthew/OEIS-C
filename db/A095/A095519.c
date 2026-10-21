/*
 * A095519  a(n) = 63 written in base 12 - n.
 * https://oeis.org/A095519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the base‑10 value of the number 63 written in base
 *            (12−n). The base is taken as an unsigned integer; base 1 (unary)
 *            is handled separately by producing 63 ones. For n ≥ 12 no valid
 *            base exists, so the function returns 0
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the base: 12 - n */
    uint64_t base = 12ULL - n;
    if (base < 1ULL) {
        /* undefined for base ≤ 0: return 0 as a sentinel value */
        return (seq_t)0;
    }
    if (base == 1ULL) {
        /* unary: 63 ones */
        seq_t r = 0;
        for (int i = 0; i < 63; ++i) {
            r = r * 10 + 1;
        }
        return r;
    }

    /* base ≥ 2: generate digits of 63 in that base */
    uint64_t value = 63ULL;
    seq_t result = 0;
    seq_t multiplier = 1;
    while (value > 0) {
        uint64_t rem = value % base;
        value /= base;
        result += (seq_t)rem * multiplier;
        multiplier *= 10;
    }
    return result;
}
