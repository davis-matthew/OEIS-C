/*
 * A037849  a(n) = Sum_{d(i) < d(i-1), i=1..m} (d(i-1) - d(i)), where
 *          Sum{d(i)*8^i: i=0,1,...,m} is the base-8 representation of n.
 * https://oeis.org/A037849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=1}^{m} (d(i‑1)-d(i)) if d(i) < d(i‑1); here d(i) are
 *            the base‑8 digits of n, d(0) being the least significant digit
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t  nn = n;              /* work on a copy of n in base‑10 */
    seq_t  old = (seq_t)(nn & 7U);   /* least significant octal digit */
    nn >>= 3;                     /* remove that digit */

    while (nn > 0) {
        seq_t next = (seq_t)(nn & 7U); /* next more significant digit */
        if (next < old)
            result += old - next;
        old = next;
        nn >>= 3;                 /* move to next digit */
    }
    return result;
}
