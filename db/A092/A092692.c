/*
 * A092692  Expansion of e.g.f. -log(1-x)/(1-x^2).
 * https://oeis.org/A092692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1..n, (n‑m) even} n! / m
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The summation uses only integers because n! is divisible by every m ≤ n. */

seq_t a(idx_t n)
{
    /* Compute n! first. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= i;

    /* Sum the terms n!/m for m with the same parity as n. */
    seq_t result = 0;
    idx_t start = (n & 1ULL) ? 1 : 2;   /* 1 if n is odd, 2 if even */
    for (idx_t m = start; m <= n; m += 2)
        result += fact / m;

    return result;
}
