/*
 * A136692  Final nonzero digit of n! in base 5.
 * https://oeis.org/A136692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n! / 5^v5(n!)) mod 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the product of numbers 1..n with all factors of 5 removed,
   then reduce that product modulo 5. */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t t = i;
        while (t % 5 == 0) t /= 5;
        res = (res * (seq_t)(t % 5)) % 5;
    }
    return res;
}
