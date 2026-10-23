/*
 * A014942  a(n) = (1 + 24^n*(23*n - 1))/529.
 * https://oeis.org/A014942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 24^n * (23*n - 1))/529
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(1) = 1; for n > 1: a(n) = a(n-1) + n*24^(n-1). */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow24  = 1;  /* 24^(k-1) while iterating k from 1 to n */

    for (idx_t k = 1; k <= n; ++k)
    {
        result += (seq_t)k * pow24;
        pow24  *= (seq_t)24;
    }

    return result;
}
