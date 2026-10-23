/*
 * A008960  Final digit of cubes: n^3 mod 10.
 * https://oeis.org/A008960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n^3 mod 10
 * Range:     u8 n <= 6, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 10 to avoid large intermediate values */
    idx_t r = n % 10ULL;          /* r is in the range 0-9 */
    seq_t r_seq = (seq_t)r;       /* Convert to seq_t for final computation */

    seq_t cube = r_seq * r_seq * r_seq; /* r^3, still <= 729 */
    return cube % 10;            /* The final digit of n^3 */
}
