/*
 * A055862  Fourth column of triangle A055858.
 * https://oeis.org/A055862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, n = 0, 1, 2 * a(n) = n² · (n+1)^(n‑2), n ≥ 3 * * The
 *            implementation uses exponentiation by squaring so that the *
 *            number of multiplications is O(log n). All intermediate results
 *            * are of type seq_t, which guarantees that the operations are *
 *            performed with the same precision as the outp...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    seq_t n_seq   = (seq_t)n;           /* integer n converted to seq_t   */
    seq_t base    = (seq_t)(n + 1);     /* (n+1) as seq_t                    */
    seq_t pow_val = 1;                  /* accumulator for (n+1)^(n-2)     */
    idx_t  exp    = n - 2;              /* exponentiation counter            */

    /* Exponentiation by squaring */
    while (exp > 0) {
        if (exp & 1)
            pow_val *= base;
        exp >>= 1;
        if (exp > 0)          /* avoid unnecessary squaring at the end   */
            base *= base;
    }

    /* Multiply by n² */
    pow_val *= n_seq;
    pow_val *= n_seq;

    return pow_val;
}
