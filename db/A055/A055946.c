/*
 * A055946  n + reversal of base 3 digits of n (written in base 10).
 * https://oeis.org/A055946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n + A030102(n)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A030102(n) is the integer obtained by reversing the base‑3 digits of n */

seq_t a(idx_t n)
{
    idx_t  temp = n;
    seq_t  rev  = 0;

    while (temp > 0) {
        rev = rev * 3 + (temp % 3);
        temp /= 3;
    }

    return (seq_t)n + rev;
}
