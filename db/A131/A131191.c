/*
 * A131191  Numbers n>=0 such that d(n) = (n^1 + 1) (n^2 + 2) ... (n^22 + 22)
 *          / 22!, e(n) = (n^1 + 1) (n^2 + 2) ... (n^23 + 23) / 23!, and f(n)
 *          = (n^1 + 1) (n^2 + 2) ... (n^24 + 24) / 24! take nonintegral
 *          values.
 * https://oeis.org/A131191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A131191: Numbers n>=0 such that 
   (∏_{k=1}^{22} (n^k + k)) / 22! is nonintegral.
   The condition reduces to: 11^2 does not divide the product, which 
   occurs exactly for n ≡ 7,18,29,40,51,62,73,84,95,106 (mod 121).
   Hence the nth term is
      a(n) = 7 + 11 * ( (n-1) + floor((n-1)/10) )
   for n ≥ 1.  This formula gives the sequence 7,18,29,40,…
   and skips the values 117, 238, 359, …
*/
seq_t a(idx_t n)
{
    /* For safety, treat n==0 as outside the defined range */
    if (n == 0) return 0;

    idx_t k      = n - 1;          /* shift to 0‑based index */
    idx_t extra  = k / 10;         /* number of skipped terms before n */
    seq_t result = (seq_t)7 + ((seq_t)11 * (seq_t)(k + extra));
    return result;
}
