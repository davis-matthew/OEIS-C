/*
 * A023385  a(n+1) = a(n) converted to base 8 from base 6 (written in base
 *          10).
 * https://oeis.org/A023385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 6; a(n+1) is obtained by writing a(n) in base 6 and
 *            interpreting that string as a base 8 number (then written in
 *            base 10)
 * Range:     u8 n <= 12, u16 n <= 17, u32 n <= 22, u64 n <= 27, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t next_value(seq_t val)
{
    seq_t result = 0;
    seq_t pow8   = 1;
    while (val > 0) {
        seq_t digit = val % 6;      /* digit in base 6, 0‑5 */
        val        /= 6;
        result += digit * pow8;    /* treat digit as base 8 */
        pow8    *= 8;              /* move to next positional place */
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset is 1; undefined for 0 */
    seq_t val = (seq_t)6;           /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        val = next_value(val);
    }
    return val;
}
