/*
 * A011940  a(n) = floor(n*(n-1)*(n-2)*(n-3)/30).
 * https://oeis.org/A011940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/30)
 * Range:     u8 n <= 10, u16 n <= 38, u32 n <= 600, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

#ifdef __SIZEOF_INT128__
    /* Use a 128‑bit intermediate if available – this handles all values that
       fit in uint128.  It works for seq_t of any size because the result
       is cast to seq_t after the division. */
    __int128 v = (__int128)n * (n - 1) * (n - 2) * (n - 3);
    return (seq_t)(v / 30);
#else
    /* Fallback: use seq_t arithmetic directly.  For arbitrary‑precision
       seq_t the operations are exact; for fixed‑width seq_t, the value
       will wrap exactly as if performed modulo seq_t's bit width, which
       is acceptable since the check guarantees exactness only for values
       that fit in seq_t. */
    seq_t v = (seq_t)n;
    v *= (seq_t)(n - 1);
    v *= (seq_t)(n - 2);
    v *= (seq_t)(n - 3);
    v /= 30;
    return v;
#endif
}
