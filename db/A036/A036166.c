/*
 * A036166  Log base 3 (n) mod 31.
 * https://oeis.org/A036166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = k, where k ∈ [0,30] satisfies 3^k ≡ n (mod 31)
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 31. */
    seq_t r = (seq_t)(n % 31);
    if (r == 0)                   /* n is a multiple of 31 – no discrete log */
        return 0;

    seq_t value = 1;               /* 3^0 mod 31 */
    for (idx_t k = 0; k < 31; ++k) /* 3 is a primitive root modulo 31 */
    {
        if (value == r)
            return (seq_t)k;      /* discrete logarithm found */
        value = (value * (seq_t)3) % 31;
    }
    /* This point is unreachable for n not divisible by 31. */
    return 0;
}
