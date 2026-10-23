/*
 * A188642  Characteristic function of numbers that are divisible by the
 *          product of their digits.
 * https://oeis.org/A188642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n is divisible by the product of its decimal digits,
 *            else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* 0 has product of digits 0 → not counted */

    unsigned long long prod = 1;
    idx_t tmp = n;
    while (tmp != 0) {
        unsigned digit = tmp % 10;
        if (digit == 0) return 0;   /* product would be 0, division undefined */
        prod *= digit;
        tmp /= 10;
    }

    /* n and prod fit in 64 bits; modulo is safe. */
    return ((unsigned long long)n % prod == 0) ? (seq_t)1 : (seq_t)0;
}
