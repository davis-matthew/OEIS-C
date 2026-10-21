/*
 * A211440  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and 2w+3x+3y=0.
 * https://oeis.org/A211440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = {10*j^2 + 6*j + 1 if n ≡ 0 (mod 3), * 10*j^2 + 10*j + 3
 *            if n ≡ 1 (mod 3), * 10*j^2 + 14*j + 5 if n ≡ 2 (mod 3), * where
 *            j = floor(n/3)
 * Range:     u8 n <= 14, u16 n <= 242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A211440 sequence value for index n.  This implementation
 * uses the explicit formulas discovered by Robert Israel, which work
 * for all non‑negative integers n and are O(1) in time. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;      /* Convert index to seq_t */
    seq_t j = N / (seq_t)3;  /* floor(n/3) */
    seq_t r = N % (seq_t)3;  /* n mod 3 */

    seq_t term;
    if (r == (seq_t)0) {
        term = (seq_t)10 * j * j + (seq_t)6 * j + (seq_t)1;
    } else if (r == (seq_t)1) {
        term = (seq_t)10 * j * j + (seq_t)10 * j + (seq_t)3;
    } else { /* r == 2 */
        term = (seq_t)10 * j * j + (seq_t)14 * j + (seq_t)5;
    }
    return term;
}
