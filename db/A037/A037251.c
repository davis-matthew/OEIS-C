/*
 * A037251  a(n) = n^3*(n^3 + 1)*(n-1).
 * https://oeis.org/A037251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * (n^3 + 1) * (n - 1), a(0)=0, a(1)=0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 23, u64 n <= 565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any seq_t (fixed‑width unsigned or arbitrary‑precision) */
/* and any idx_t (uint64_t). */
seq_t a(idx_t n)
{
    /* For n = 0 or 1 the formula would use (n-1) < 0, so return 0 directly. */
    if (n < 2) return 0;

    seq_t ns  = (seq_t)n;              /* cast index to seq_t */
    seq_t ns_sq = ns * ns;             /* n^2   */
    seq_t ns_cu = ns_sq * ns;          /* n^3   */
    seq_t term  = ns_cu * (ns_cu + 1); /* n^3 * (n^3 + 1) */
    return term * (ns - 1);            /* * (n-1) */
}
