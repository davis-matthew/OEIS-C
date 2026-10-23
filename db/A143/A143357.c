/*
 * A143357  Floor((n-1)!/[n(n+1)]).
 * https://oeis.org/A143357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1)! / (n*(n+1)))
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 15, u64 n <= 23, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate factorial.
       This is sufficient for all n where the result fits
       into seq_t on builds that use 64‑bit seq_t.  For
       larger seq_t implementations the compiler will
       promote the literals to the appropriate type. */
    unsigned __int128 num = 1;
    for (uint64_t i = 2; i < (uint64_t)n; ++i)
        num *= i;

    unsigned __int128 denom = (unsigned __int128)n * ((unsigned __int128)n + 1);
    return (seq_t)(num / denom);
}
