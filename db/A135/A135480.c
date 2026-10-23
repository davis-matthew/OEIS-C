/*
 * A135480  Generator for the finite sequence A038178.
 * https://oeis.org/A135480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/10)*(n-(n mod 10))*(729*n^2-14879*n+75971) + (n mod
 *            10)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 237, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nm   = (seq_t)n;          /* cast index to seq_t once */
    seq_t mod10 = nm % 10;          /* n mod 10 */
    seq_t base   = nm - mod10;      /* n - (n mod 10) */
    seq_t term   = 729 * nm * nm    /* 729*n^2 */
                 - 14879 * nm       /* -14879*n */
                 + 75971;           /* +75971 */
    seq_t prod   = (base * term) / 10;   /* divide by 10 (truncating) */
    return prod + mod10;                    /* add the remainder */
}
