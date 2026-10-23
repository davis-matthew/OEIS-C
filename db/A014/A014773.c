/*
 * A014773  Squares of odd heptagonal numbers.
 * https://oeis.org/A014773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((5k^2-3k)/2)^2 with k = 4*⌊(n-1)/2⌋ + (n mod 2?1:2),
 *            i.e. the square of the n‑th odd heptagonal number
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 82, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n = 0; offset is 1 */

    /* compute k = 4*floor((n-1)/2) + (n%2?1:2) */
    idx_t m = (n - 1) >> 1;                 /* floor((n-1)/2) */
    idx_t k = (m << 2) + ((n & 1) ? 1 : 2); /* k based on parity of n */

    seq_t ks = (seq_t)k;
    seq_t hk = (5 * ks * ks - 3 * ks) / 2;  /* the odd heptagonal number */
    return hk * hk;                         /* its square */
}
