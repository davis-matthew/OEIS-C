/*
 * A095483  a(n) = 45 written in base 15 - n.
 * https://oeis.org/A095483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = digits of 45 written in base (15‑n), interpreted as a
 *            decimal integer
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle only defined range n = 0 … 14. For n > 14
       return 0 (undefined behaviour for the sequence). */
    if (n > 14u)
        return 0;

    idx_t base = 15u - n;   /* base ≥ 1 for n ≤ 14 */

    /* Base 1 (unary) representation of 45:
       the decimal digits are 45 copies of '1'. */
    if (base == 1u) {
        seq_t r = 0;
        for (idx_t i = 0; i < 45u; ++i)
            r = r * (seq_t)10 + (seq_t)1;
        return r;
    }

    /* For base ≥ 2: convert 45 to the given base,
       collect the digits (least‑significant first). */
    idx_t t = 45u;
    int digits[10];  /* enough for all cases (max 6 digits). */
    int cnt = 0;
    while (t > 0u) {
        digits[cnt++] = (int)(t % base);
        t /= base;
    }

    /* Build the decimal integer from the digit sequence. */
    seq_t r = 0;
    for (int i = cnt - 1; i >= 0; --i)
        r = r * (seq_t)10 + (seq_t)digits[i];

    return r;
}
