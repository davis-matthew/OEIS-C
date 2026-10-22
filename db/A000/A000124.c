/*
 * A000124  Central polygonal numbers (the Lazy Caterer's sequence): n(n+1)/2
 *          + 1; or, maximal number of pieces formed when slicing a pancake
 *          with n cuts.
 * https://oeis.org/A000124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice, changed
 * Formula:   a(n) = n*(n+1)/2 + 1
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t;
    if (n % 2 == 0)
        t = (seq_t)(n / 2) * ((seq_t)n + 1);
    else
        t = (seq_t)n * (seq_t)((n + 1) / 2);
    return t + 1;
}
