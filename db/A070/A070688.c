/*
 * A070688  a(n) = n^6 mod 48.
 * https://oeis.org/A070688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 48
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 48 first to keep intermediates small */
    const seq_t MOD = 48;
    seq_t r = 1;
    seq_t x = (seq_t)n % MOD;
    /* Compute x^6 mod 48 by repeated multiplication */
    r = (r * x) % MOD;
    r = (r * x) % MOD;
    r = (r * x) % MOD;
    r = (r * x) % MOD;
    r = (r * x) % MOD;
    r = (r * x) % MOD;
    return r;
}
