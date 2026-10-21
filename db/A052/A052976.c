/*
 * A052976  Expansion of (1-2x)/(1-3x-x^3+2x^4).
 * https://oeis.org/A052976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+4) = a(n+1) + 3*a(n+3) – 2*a(n), a(0)=1, a(1)=1, a(2)=3,
 *            a(3)=10
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the expansion of (1-2x)/(1-3x-x^3+2x^4) as defined by A052976 */
seq_t a(idx_t n)
{
    /* Small indices: return hard‑coded initial values */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 10;

    /* Rolling window of the last four terms */
    seq_t a0 = 1;          /* a(n)   */
    seq_t a1 = 1;          /* a(n+1) */
    seq_t a2 = 3;          /* a(n+2) */
    seq_t a3 = 10;         /* a(n+3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a1 + 3 * a3 - 2 * a0; /* a(n+4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3; /* a(n) */
}
