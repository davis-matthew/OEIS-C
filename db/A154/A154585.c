/*
 * A154585  a(n) = abs(Sum_{k=1..n} (-1)^k * (n-k+1 mod k)).
 * https://oeis.org/A154585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = abs( Σ_{k=1..n} (-1)^k * ((n-k+1) mod k) )
 * Range:     u8 n <= 238, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use an ordinary signed integer for the intermediate sum.  The
       maximum magnitude for n up to 10 000 is below 50 000 000,
       comfortably inside a 32‑bit signed value.  The result is
       converted to seq_t at the end. */
    int sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* (n-k+1) is always non‑negative because k ≤ n. */
        idx_t rem = (n - k + 1) % k;
        int term = (int)rem;
        if ((k & 1) == 0)        /* even k -> subtract */
            sum -= term;
        else                     /* odd k  -> add    */
            sum += term;
    }
    int abs_sum = sum < 0 ? -sum : sum;
    return (seq_t)abs_sum;
}
