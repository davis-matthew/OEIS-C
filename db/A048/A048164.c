/*
 * A048164  a(0)=1, a(n+1)=1+(2^(2^n)+1)*a(n).
 * https://oeis.org/A048164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0) = 1; a(n+1) = 1 + (2^(2^n) + 1) * a(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_val = 1;      /* a(0) */
    seq_t pow2   = 2;     /* 2^(2^0) */

    for (idx_t i = 0; i < n; ++i)
    {
        seq_t term = 1 + (pow2 + 1) * a_val; /* a(i+1) */
        a_val = term;
        pow2 = pow2 * pow2;                  /* 2^(2^(i+1)) */
    }
    return a_val;
}
