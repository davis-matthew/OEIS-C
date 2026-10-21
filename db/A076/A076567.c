/*
 * A076567  Greatest prime divisor of 4n+6 (sum of four successive integers).
 * https://oeis.org/A076567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = greatest prime divisor of (4n+6)
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the largest prime factor of an unsigned 64‑bit integer.
   This helper is used because the numbers we factor (4n+6) fit
   comfortably in 64 bits for all test data. */
static seq_t _gpf_uint64(uint64_t n)
{
    uint64_t maxp = 1;
    while ((n & 1ULL) == 0)      /* factor out 2 */
    {
        maxp = 2;
        n >>= 1;
    }
    for (uint64_t d = 3; d * d <= n; d += 2)
    {
        if (n % d == 0)
        {
            maxp = d;
            while (n % d == 0)
                n /= d;
        }
    }
    if (n > 1)
        maxp = n;                /* remaining prime factor */
    return (seq_t)maxp;
}

seq_t a(idx_t n)
{
    /* 4n+6 fits in 64 bits for the intended range of n. */
    uint64_t val = (uint64_t)n * 4ULL + 6ULL;
    return _gpf_uint64(val);
}
