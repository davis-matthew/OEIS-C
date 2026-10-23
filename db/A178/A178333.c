/*
 * A178333  Characteristic function of mountain numbers.
 * https://oeis.org/A178333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = if n mod 10 = 1 then if n = 1 then 1 else g(n div 10, 1)
 *            else 0 with g(x, y) = if x mod 10 > y then g(x div 10, x mod 10)
 *            else if x mod 10 = y then 0 else h(x div 10, x mod 10) h(x, y) =
 *            if y = 1 then 0^x else if x mod 10 < y then h(x div 10, x mod
 *            10) else 0 (0^0 is treated as 1)
 * Range:     u8 n <= 9890, u16 n <= 9890, u32 n <= 9890, u64 n <= 9890,
 *            u128 n <= 9890, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t h(idx_t x, unsigned char y) {
    if (y == 1)
        return (x == 0) ? (seq_t)1 : (seq_t)0;
    unsigned char d = (unsigned char)(x % 10);
    if (d < y)
        return h(x / 10, d);
    return (seq_t)0;
}

static seq_t g(idx_t x, unsigned char y) {
    unsigned char d = (unsigned char)(x % 10);
    if (d > y)
        return g(x / 10, d);
    if (d == y)
        return (seq_t)0;
    return h(x / 10, d);
}

seq_t a(idx_t n) {
    if ((n % 10) != 1)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    return g(n / 10, 1);
}
