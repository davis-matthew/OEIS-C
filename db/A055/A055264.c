/*
 * A055264  Possible values of A055263; numbers equal to 0, 1, 3 or 6 modulo
 *          9.
 * https://oeis.org/A055264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 9*floor(n/4) + (n mod 4)*(1 + (n mod 4))/2
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A055264, the possible digit sums of a triangular number. */
seq_t a(idx_t n)
{
    /* n is a 64‑bit unsigned index. Convert parts needed to seq_t. */
    seq_t n_mod4 = (seq_t)(n % 4);            /* 0,1,2,3 */
    seq_t quotient = (seq_t)(n / 4);          /* floor(n/4) */
    /* half the product (n_mod4 * (1 + n_mod4)) is always an integer. */
    seq_t add = n_mod4 * ((seq_t)1 + n_mod4) / 2;

    /* Result: 9 * floor(n/4) + add */
    return (seq_t)9 * quotient + add;
}
