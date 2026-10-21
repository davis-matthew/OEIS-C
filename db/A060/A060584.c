/*
 * A060584  Compare ultimate and penultimate digits of n base 3, i.e., 0 if n
 *          mod 3 = floor(n/3) mod 3, 1 otherwise; also 0 if (n mod 9) is a
 *          multiple of 4, 1 otherwise.
 * https://oeis.org/A060584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 0 if n mod 9 is 0, 4 or 8, otherwise 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Hence a(n + 9) = a(n).                                          */
seq_t a(idx_t n)
{
    idx_t r = n % 9;                 /* r ∈ {0,…,8} */
    return (r == 0 || r == 4 || r == 8) ? (seq_t)0 : (seq_t)1;
}
