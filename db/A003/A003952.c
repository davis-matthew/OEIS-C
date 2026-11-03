/*
 * A003952  Expansion of g.f.: (1+x)/(1-9*x).
 * https://oeis.org/A003952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*9^(n-1) for n >= 1, a(0) = 1 (equivalently a(n) =
 *            (10*9^n - 0^n)/9)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 974 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a003952_pow9(idx_t e) {
    seq_t result = (seq_t)1;
    seq_t base = (seq_t)9;
    while (e > 0) {
        if (e & 1)
            result *= base;
        e >>= 1;
        if (e > 0)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    return (seq_t)10 * a003952_pow9(n - 1);
}
