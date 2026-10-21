/*
 * A066209  A053041(n)-10^(n-1).
 * https://oeis.org/A066209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n - 10^(n-1) mod n) mod n // offset to make 10^(n-1)
 *            divisible by n
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for indices < 1, not used normally */
    idx_t mod = n;
    idx_t res = 1 % mod;                 /* 10^0 mod n */
    idx_t base = 10 % mod;               /* 10^1 mod n */
    idx_t exp = n - 1;
    while (exp)
    {
        if (exp & 1U)
            res = ( (unsigned __int128) res * base ) % mod;
        base = ( (unsigned __int128) base * base ) % mod;
        exp >>= 1U;
    }
    if (res == 0) return 0;              /* already divisible */
    return (seq_t)(mod - res);           /* smallest non‑negative x adding to 10^(n-1) */
}
