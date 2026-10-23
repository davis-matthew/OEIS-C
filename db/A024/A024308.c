/*
 * A024308  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k=[
 *          (n+1)/2 ], s = (natural numbers >= 2), t = A023532.
 * https://oeis.org/A024308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} (i+1) · (1 - [n+2-i is a triangular
 *            number])
 * Range:     u8 n <= 44, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = (n + 1) >> 1;                     /* k = ⌊(n+1)/2⌋               */
    seq_t sumAll = (seq_t)k * ((seq_t)k + 3) / 2; /* Σ_{i=1}^{k} (i+1)          */
    seq_t res   = sumAll;

    /* subtract contributions of indices where t(n+1-i)=0,
       i.e. where n+2-i is a triangular number */
    for (idx_t m = 1; ; ++m)
    {
        idx_t T = m * (m + 1) / 2;              /* triangular number T_m      */
        if (T > n + 1)
            break;
        if (T >= n + 2 - k)                     /* T lies within the relevant range */
            res -= (seq_t)(n + 3 - T);          /* subtract (i+1) = n+3-T       */
    }

    return res;
}
