/*
 * A062807  a(n) = Sum_{i=1..n} i*(n-i)^i.
 * https://oeis.org/A062807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} i * (n-i)^i
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute a(n) directly; no memoisation is needed because the
       cost is acceptable up to n ≈ 10000. */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* base = n - i */
        seq_t base = (seq_t)(n - i);
        /* fast exponentiation: base^i */
        seq_t power = 1;
        idx_t e = i;
        while (e)
        {
            if (e & 1u)
                power *= base;
            base *= base;
            e >>= 1;
        }
        sum += (seq_t)i * power;
    }
    return sum;
}
