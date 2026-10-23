/*
 * A159617  G.f.: (1-x)/(1-8*x-8*x^2+8*x^3).
 * https://oeis.org/A159617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 8*a(n-2) - 8*a(n-3) for n>2, * with a(0) = 1,
 *            a(1) = 7, a(2) = 64
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A159617.  The function
 * is safe for any order of calls and works for any supported
 * seq_t type. */
seq_t a(idx_t n)
{
    /* Handle the small initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)64;

    /* Use three variables to keep the last three terms. */
    seq_t a0 = (seq_t)1;  /* a(n-3) */
    seq_t a1 = (seq_t)7;  /* a(n-2) */
    seq_t a2 = (seq_t)64; /* a(n-1) */
    seq_t an;            /* a(n) */

    /* Iterate from n = 3 up to the desired index. */
    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)8 * a2 + (seq_t)8 * a1 - (seq_t)8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2; /* a(n) */
}
