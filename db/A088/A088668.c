/*
 * A088668  Number of n X n matrices over GF(2) with characteristic polynomial
 *          x^(n-1) * (x-1).
 * https://oeis.org/A088668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A088668: Number of n×n matrices over GF(2) with characteristic polynomial
   x^(n‑1)·(x‑1)
   Formula:  a(n) = 2^(n^2‑n+1) – 2^(n^2‑2n+1)   (n ≥ 1)
   (Same as a(n) = 2*(2^(n^2‑n) – 2^(n^2‑2n)) for non‑negative exponents.)   */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n = 0 we return 0 to keep the
       function type stable. No special handling for n = 1 is needed,
       because the exponents become (1‑1+1)=1 and (1‑2+1)=0.            */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Compute the two exponents.  For typical use n is small enough that
       n*n fits in idx_t (uint64_t).  If larger values are required the
       multiplication could be carried out in an unsigned __int128.     */
    idx_t exp1 = n * n - n + 1;         /* n^2 – n + 1  */
    idx_t exp2 = n * n - (idx_t)2 * n + 1; /* n^2 – 2n + 1 */

    /* For arbitrary‑precision seq_t the shift operator is overloaded.
       For small seq_t types the shift works as usual bit‑shifts.     */
    seq_t term1 = ((seq_t)1 << exp1);
    seq_t term2 = ((seq_t)1 << exp2);

    return term1 - term2;
}
