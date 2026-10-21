/*
 * A077842  Expansion of (1-x)/(1-2*x-2*x^2-3*x^3).
 * https://oeis.org/A077842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 1, a(2) = 4,
 * a(n) = 2*a(n-1) + 2*a(n-2) + 3*a(n-3)  for n ≥ 3 */

/* Compute term a(n) of OEIS sequence A077842. */
seq_t a(idx_t n)
{
    /* Handle trivial base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Iteratively build the recurrence up to n.
       The computation uses only integer addition and
       multiplication by small constants, so it is valid
       for all supported seq_t types (fixed‑width or
       arbitrary‑precision). */
    seq_t a0 = (seq_t)1; /* a(n-3) */
    seq_t a1 = (seq_t)1; /* a(n-2) */
    seq_t a2 = (seq_t)4; /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = 2*a2 + 2*a1 + 3*a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
