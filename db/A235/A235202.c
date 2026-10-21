/*
 * A235202  Numbers written in an alternating binary-then-quaternary base.
 * https://oeis.org/A235202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the number obtained by writing n in mixed radix with
 *            bases alternating 2,4 (least significant digit base=2). The
 *            digits are then interpreted as decimal digits and concatenated
 *            from most to least significant. Equivalent to repeatedly taking
 *            n mod 2, n/=2, n mod 4, n/=4, ... and building th...
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* mixed‑radix digits (least significant first) */
    idx_t temp = n;
    int digits[128];
    int len = 0;
    idx_t base = 2;
    while (temp > 0) {
        idx_t q = temp / base;
        idx_t rem = temp % base;
        digits[len++] = (int)rem;
        temp = q;
        base = (base == 2 ? 4 : 2);
    }
    /* assemble decimal digits into the result */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }
    return result;
}
