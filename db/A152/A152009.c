/*
 * A152009  (L)-sieve transform of {1,4,7,10,...,3n-2,...} (A016777).
 * https://oeis.org/A152009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3*a(n-1)+3)/2), a(1) = 1
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1, but never used */

    seq_t prev = (seq_t)1;          /* a(1) */
    if (n == 1) return prev;

    for (idx_t i = 2; i <= n; ++i) {
        /*  a(i) = floor((3*a(i‑1)+3)/2)  */
        prev = ((seq_t)3 * prev + (seq_t)3) / (seq_t)2;
    }

    return prev;
}
