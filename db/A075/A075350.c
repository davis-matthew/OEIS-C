/*
 * A075350  1-1, 2*3-(2+3), 4*5*6-(4+5+6), 7*8*9*10-(7+8+9+10), ...
 * https://oeis.org/A075350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π[j = n(n‑1)/2 + 1 .. n(n+1)/2] j – (n(n²+1))/2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (Defined for n ≥ 1; a(1) = 0.) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* graceful handling for non‑positive indices */

    /* Compute product of the consecutive integers in the range. */
    idx_t start = n * (n - 1) / 2 + 1;
    idx_t end   = n * (n + 1) / 2;

    seq_t prod = (seq_t)1;
    for (idx_t i = start; i <= end; ++i)
        prod *= (seq_t)i;           /* multiplication in seq_t */

    /* Subtract the sum of the same range:
       sum = n * (n² + 1) / 2  (derived from the arithmetic series formula) */
    seq_t nn   = (seq_t)n;
    seq_t sum  = nn * ((nn * nn) + (seq_t)1) / (seq_t)2;

    return prod - sum;
}
