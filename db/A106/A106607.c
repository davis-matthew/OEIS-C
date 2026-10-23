/*
 * A106607  Expansion of (1+t^3)^2/((1-t)*(1-t^2)^2*(1-t^4)).
 * https://oeis.org/A106607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 18*n^2 + 56*n + {96,18,72,42}[n % 4]) / 96
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is defined for all non‑negative n; no memoisation is needed. */
seq_t a(idx_t n)
{
    /* Cast the index to the result type before doing arithmetic. */
    seq_t nn = (seq_t)n;

    /* Compute n^2 and n^3. */
    seq_t nn2 = nn * nn;
    seq_t term = 4 * nn2 * nn + 18 * nn2 + 56 * nn;

    /* Add the periodic correction depending on n mod 4. */
    idx_t mod = n & 3UL;        /* n % 4  (fast for uint64_t) */
    seq_t periodic;
    if (mod == 0)      periodic = (seq_t)96;
    else if (mod == 1) periodic = (seq_t)18;
    else if (mod == 2) periodic = (seq_t)72;
    else               periodic = (seq_t)42;   /* mod == 3 */

    /* Final division – the numerator is always divisible by 96. */
    return (term + periodic) / (seq_t)96;
}
