/*
 * A070621  a(n) = n^5 mod 38.
 * https://oeis.org/A070621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 38
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 38 first – the result depends only on this value. */
    const seq_t MOD   = (seq_t)38;
    seq_t   base     = (seq_t)(n % 38UL);   /* base < 38 */
    seq_t   result   = 1;                   /* 1 < 38 */

    /* Compute base^5 mod 38.  Use a wider integer type for the temporary
       multiplication so that a 1‑byte (e.g. uint8_t) seq_t cannot overflow. */
    for (int i = 0; i < 5; ++i)
    {
        unsigned long long temp = (unsigned long long)result *
                                  (unsigned long long)base;
        result = (seq_t)(temp % 38ULL);
    }
    return result;
}
