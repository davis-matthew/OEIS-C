/*
 * A008265  Coordination sequence T1 for Zeolite Code ATO.
 * https://oeis.org/A008265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(5m) = 57m² + 2, a(5m+1) = 57m² + 24m + 4, a(5m+2) = 57m² + 46m
 *            + 11, a(5m+3) = 57m² + 68m + 22, a(5m+4) = 57m² + 90m + 37, with
 *            a(0) = 1
 * Range:     u8 n <= 10, u16 n <= 169, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    idx_t m = n / 5;
    idx_t r = n % 5;
    seq_t mm = (seq_t)m;
    seq_t base = mm * mm;              /* m² */
    switch (r) {
    case 0: return base * 57 + 2;
    case 1: return base * 57 + 24 * mm + 4;
    case 2: return base * 57 + 46 * mm + 11;
    case 3: return base * 57 + 68 * mm + 22;
    case 4: return base * 57 + 90 * mm + 37;
    }
    return 0;   /* unreachable */
}
