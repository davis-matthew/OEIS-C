/*
 * A004346  Binomial coefficient C(5n,n-4).
 * https://oeis.org/A004346
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(5n, n-4) = Product_{i=1..n-4} (4n+4+i)/i
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_s(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    idx_t k = n - 4;
    seq_t base = (seq_t)4 * (seq_t)n + 4;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t si = (seq_t)i;
        seq_t x = base + si;
        seq_t g = gcd_s(r, si);
        r /= g;
        r *= x / (si / g);
    }
    return r;
}
