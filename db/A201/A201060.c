/*
 * A201060  Repdigit numbers with repdigit digital product.
 * https://oeis.org/A201060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 17, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Repdigit numbers with repdigit digital product.
 *   a(n) = n-1 for 1 ≤ n ≤ 10
 *   a(11) = 11   a(12) = 22   a(13) = 33
 *   a(14) = 111  a(15) = 222
 *   a(n) for n ≥ 16 is the repunit of length n-12:
 *   a(n) = 1…1 (n-12 times)
 */
seq_t a(idx_t n)
{
    if (n <= 10)                /* 0–9 */
        return (seq_t)(n - 1);

    switch (n) {
        case 11: return 11;
        case 12: return 22;
        case 13: return 33;
        case 14: return 111;
        case 15: return 222;
    }

    /* n ≥ 16 : construct repunit of length n-12 */
    idx_t len = n - 12;
    seq_t rep = 0;
    for (idx_t i = 0; i < len; ++i) {
        rep = rep * 10 + 1;
    }
    return rep;
}
