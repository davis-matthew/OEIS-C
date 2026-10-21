/*
 * A068177  a(1) = 3; for n>1, a(n) is the smallest square > a(n-1) obtained
 *          by inserting digits into a(n-1).
 * https://oeis.org/A068177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068177:  a(1)=3, and for n>1
   a(n) is the smallest square greater than a(n-1) that can be
   obtained by inserting digits into a(n-1).  The terms are
   3, 36, 361, 32761, 323761, 32137561, 3211375561, ….
   These squares are produced by the squares of
     6,
     19,
     181,
   and for n≥5 by squaring the number consisting of a leading
   5, followed by (n‑4) copies of the digit 6, and a trailing 9. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) { seq_t r = (seq_t)6; return r * r; }
    if (n == 3) { seq_t r = (seq_t)19; return r * r; }
    if (n == 4) { seq_t r = (seq_t)181; return r * r; }

    /* For n ≥ 5: construct root = 5 followed by (n‑4) sixes then 9 */
    seq_t root = (seq_t)5;
    idx_t k = n - 4;                       /* number of 6's */
    for (idx_t i = 0; i < k; ++i)
        root = root * (seq_t)10 + (seq_t)6;
    root = root * (seq_t)10 + (seq_t)9;

    return root * root;                    /* square of the constructed root */
}
