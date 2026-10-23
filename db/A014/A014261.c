/*
 * A014261  Numbers that contain odd digits only.
 * https://oeis.org/A014261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = Σ_{j≥0} (2*d_j(n) – 1) * 10^j where d_j(n) are the digits
 *            (1…5) of the zerofree base‑5 expansion of n. The zerofree base‑5
 *            digits are obtained by repeatedly taking r = n % 5; if r == 0
 *            set r = 5, then n = (n – r) / 5
 * Range:     u8 n <= 55, u16 n <= 2655, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required constants and types (seq_t, idx_t) are supplied by the build */

seq_t a(idx_t n)
{
    if (n == 0)           /* offset starts at 1; n==0 is undefined */
        return 0;

    seq_t result = 0;
    seq_t pow10 = 1;
    idx_t temp = n;

    while (temp > 0)
    {
        idx_t r = temp % 5;          /* remainder 0..4 */
        if (r == 0)                  /* zero digit → 5 */
            r = 5;

        seq_t digit_odd = (seq_t)(2 * r - 1);  /* maps 1→1, 2→3, …, 5→9 */

        result += digit_odd * pow10;
        pow10 *= 10;

        temp = (temp - r) / 5;
    }
    return result;
}
