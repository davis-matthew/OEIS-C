/*
 * A178187  Numerators of sum (1/3)^((k^2+3k)/2) from k=1 to n.
 * https://oeis.org/A178187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(1) = 1, a(n) = 3^(n+1) * a(n-1) + 1 for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A178187.
   The value is returned as a seq_t. The function uses only
   integer arithmetic allowed for seq_t. */
seq_t a(idx_t n)
{
    if (n < 1) /* behaviour for n < 1 is not required */
        return 0;

    seq_t res = 1;   /* a(1) */

    if (n == 1)
        return res;

    seq_t pow3 = 9;  /* 3^2, will be 3^(i+1) inside the loop */

    for (idx_t i = 2; i <= n; ++i) {
        pow3 = pow3 * 3;              /* pow3 = 3^(i+1)   */
        res = res * pow3 + 1;         /* a(i) = 3^(i+1)*a(i-1)+1 */
    }
    return res;
}
