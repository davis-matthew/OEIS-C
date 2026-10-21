/*
 * A079250  Even numbers in A079000.
 * https://oeis.org/A079250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A079250: Even numbers in A079000
   The sequence is
      4, 6, 8, 16, 18, 20, 34, 36, 38, 40, 42, 44, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, ...
   For n ≥ 4 let k be the smallest integer such that 3·2^k ≥ n.
   Then
          a(n) = 6·2^k + 2·n – 4.
   For n = 1,2,3: a(1)=4, a(2)=6, a(3)=8. */

seq_t a(idx_t n)
{
    /* Handle the first three special terms */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)8;

    /* Compute k such that 3*2^k ≥ n.  Equivalent: 2^k ≥ ceil(n/3). */
    idx_t m = n / 3;          /* floor division */
    if (n % 3 != 0) ++m;      /* ceil division */

    /* Find the smallest k with 2^k ≥ m.  Use seq_t to avoid overflow. */
    seq_t val = (seq_t)1;     /* 2^0 */
    int k = 0;
    while (val < (seq_t)m)
    {
        val <<= 1;            /* val = 2^(k+1) */
        ++k;
    }

    /* Compute the result using the formula a(n) = 6*2^k + 2*n – 4. */
    seq_t power = (seq_t)1 << k;            /* 2^k */
    seq_t term = ((seq_t)6) * power          /* 6·2^k */
               + ((seq_t)2) * n           /* + 2·n   */
               - ((seq_t)4);             /* - 4     */

    return term;
}
