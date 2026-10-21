/*
 * A207598  Number of n X 7 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 120*n^4 + 40*n^3 - 200*n^2 + 80*n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 77, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is a polynomial of degree 4, valid for all n >= 1. */
seq_t a(idx_t n)
{
    /* cast index to seq_t first */
    seq_t t  = (seq_t)n;      /* n */
    seq_t t2 = t * t;        /* n^2 */
    seq_t t3 = t2 * t;       /* n^3 */
    seq_t t4 = t3 * t;       /* n^4 */

    seq_t ans = ((seq_t)120) * t4;  /* 120*n^4 */
    ans += ((seq_t)40)  * t3;       /* +40*n^3 */
    ans += ((seq_t)80)  * t;        /* +80*n   */
    ans -= ((seq_t)200) * t2;       /* -200*n^2 */

    return ans;
}
