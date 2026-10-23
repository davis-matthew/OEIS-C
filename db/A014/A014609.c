/*
 * A014609  a(n) = (5n)!/(5!^n).
 * https://oeis.org/A014609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5n)!/(5!^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t res = (seq_t)1;
    const seq_t fact5 = (seq_t)120;          /* 5! */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t term = (seq_t)1;
        idx_t base = 5 * i;                  /* 5i */
        /* compute C(5i,5) = (5i)(5i-1)(5i-2)(5i-3)(5i-4) / 5! */
        for (idx_t j = 0; j < 5; ++j)
            term *= (seq_t)(base - j);
        term /= fact5;                       /* exact division */
        res *= term;
    }
    return res;
}
