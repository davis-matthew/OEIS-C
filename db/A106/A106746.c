/*
 * A106746  Replace each digit d of n with floor(d/2) + 5.
 * https://oeis.org/A106746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = replace each decimal digit d of n by floor(d/2)+5
 * Range:     u8 n <= 99, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 89 terms through n = 4720 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements directly the definition without lookup. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)5;

    /* n fits in uint64_t, so at most 20 decimal digits */
    idx_t digits[20];
    idx_t len = 0;
    idx_t tmp = n;

    /* extract digits least‑significant first */
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    seq_t result = 0;
    /* process from most‑significant to least‑significant */
    for (idx_t i = 0; i < len; ++i) {
        unsigned d = (unsigned)digits[len - 1 - i];
        unsigned newdigit = (d / 2u) + 5u;   /* floor(d/2)+5, always in 5..9 */
        result = result * (seq_t)10 + (seq_t)newdigit;
    }

    return result;
}
