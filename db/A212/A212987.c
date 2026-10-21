/*
 * A212987  Number of (w,x,y) with all terms in {0,...,n} and 3*w = 2*x+2*y.
 * https://oeis.org/A212987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212987:
 *   a(n) = number of triples (w,x,y) ∈ {0,…,n}³ with 3*w = 2*x + 2*y.
 * For each even w = 2k (k = 0 … ⌊n/2⌋) the equation becomes x + y = 3k.
 * The number of pairs (x,y) with 0 ≤ x,y ≤ n and x + y = S is
 *   S + 1          if 0 ≤ S ≤ n,
 *   2n – S + 1     if n < S ≤ 2n,
 *   0              otherwise.
 * Summing this over all admissible k yields the desired value.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n >> 1;            /* floor(n/2) */
    seq_t two_n = (seq_t)n + (seq_t)n;  /* 2n in seq_t */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t s = 3 * k;            /* sum x + y = 3k */
        seq_t s_se = (seq_t)s;

        if (s <= n) {
            sum += s_se + 1;        /* S + 1 solutions */
        } else {
            sum += two_n - s_se + 1; /* 2n – S + 1 solutions */
        }
    }
    return sum;
}
