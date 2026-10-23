/*
 * A034125  Decimal part of cube root of n starts with digit 9.
 * https://oeis.org/A034125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth positive integer m such that the first decimal
   digit of the cube root of m is 9.  For each integer part k of a cube
   root, the admissible m lie in the interval
   [ ceil((k+0.9)^3) , (k+1)^3 – 1 ].
   The function enumerates these intervals until the cumulative count
   reaches n, then returns the appropriate value.  All arithmetic is
   performed with integer types only (no floating‑point).  */

seq_t a(idx_t n)
{
    /* Treat the remaining target as a seq_t to avoid mixing types. */
    seq_t n_remaining = (seq_t)n;

    for (idx_t k = 1; ; ++k)
    {
        /* base = 10*k + 9;  base^3 = (10*k+9)^3. */
        seq_t base   = (seq_t)k * 10 + 9;
        seq_t base3  = base * base * base;
        /* lower = ceil(base3 / 1000) = (base3 + 999) / 1000. */
        seq_t lower  = (base3 + 999) / 1000;

        /* upper = (k+1)^3 – 1. */
        seq_t upper  = (seq_t)(k + 1);
        upper = upper * upper * upper - 1;

        if (upper < lower)
            continue;                      /* no numbers for this k */

        /* count of integers in this interval. */
        seq_t count = upper - lower + 1;

        if (n_remaining <= count)
            return lower + n_remaining - 1;/* nth number found */

        n_remaining -= count;            /* skip this whole interval */
    }
}
