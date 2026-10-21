/*
 * A081815  Decimal expansion of electron charge to mass quotient (negated).
 * https://oeis.org/A081815
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = the (n-12)-th digit of the integer 175882000000 counted
 *            from the most significant digit; if n ≥ offset+len return 0.
 *            (offset = 12, len = 12)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const seq_t num = (seq_t)175882000000ULL; /* 175 882 000 000 */
    const idx_t offset = 12;
    const idx_t len = 12; /* number of decimal digits in num */

    if (n < offset) {
        /* indices below the offset are considered undefined; return 0 */
        return (seq_t)0;
    }

    idx_t pos = n - offset;   /* position in the digit string, 0‑based */
    if (pos >= len) {
        /* beyond the known digits: all remaining terms are 0 */
        return (seq_t)0;
    }

    /* Compute 10^(len-1-pos) */
    idx_t exp = len - 1 - pos;
    seq_t divisor = 1;
    while (exp--) divisor *= 10;

    seq_t digit = (num / divisor) % 10;
    return digit;
}
