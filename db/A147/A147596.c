/*
 * A147596  a(n) is the number whose binary representation is A138145(n).
 * https://oeis.org/A147596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 7*(2^(n-3)+1) for n ≥ 6; a(n) = 2^n – 1 for 1 ≤ n ≤ 5
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_of_two(idx_t exp) {
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        r = r * (seq_t)2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n <= (idx_t)5) {
        seq_t r = power_of_two(n);
        return r - (seq_t)1;
    } else {
        seq_t r = power_of_two(n - (idx_t)3);
        return (seq_t)7 * (r + (seq_t)1);
    }
}
