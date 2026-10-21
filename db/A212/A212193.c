/*
 * A212193  In ternary representation of n: a(n) = if n is pandigital then 3
 *          else least digit not used.
 * https://oeis.org/A212193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3 if ternary(n) contains all digits 0,1,2; * otherwise
 *            a(n) = the minimum of {0,1,2}\setminus{digits of ternary(n)}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned mask = 0;
    idx_t t = n;

    if (t == 0) {
        /* ternary representation of 0 is "0" */
        mask = 1 << 0;
    } else {
        while (t != 0) {
            unsigned d = t % 3;       /* d ∈ {0,1,2} */
            mask |= 1 << d;
            t /= 3;
        }
    }

    /* if all three digits are present, n is pandigital */
    if (mask == 0b111) {
        return (seq_t)3;
    }

    /* otherwise return the least missing digit */
    for (unsigned d = 0; d < 3; ++d) {
        if ((mask & (1 << d)) == 0) {
            return (seq_t)d;
        }
    }

    /* should never reach here */
    return (seq_t)0;
}
