/*
 * A037680  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,0,2,3.
 * https://oeis.org/A037680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A037680
 *  a(n) = 11*a(n-1) - 11*a(n-2) + 11*a(n-3) - 10*a(n-4)
 *  with initial values a(1)=1, a(2)=10, a(3)=102, a(4)=1023.
 *  (Digits of the periodic sequence 1,0,2,3.)
 */

seq_t a(idx_t n)
{
    /* safety for n < 1; the sequence is defined for n ≥ 1 */
    if (n < 1)
        return 0;

    /* initial values */
    seq_t a1 = 1u;   /* a(1) */
    if (n == 1U) return a1;
    seq_t a2 = 10u;  /* a(2) */
    if (n == 2U) return a2;
    seq_t a3 = 102u; /* a(3) */
    if (n == 3U) return a3;
    seq_t a4 = 1023u;/* a(4) */
    if (n == 4U) return a4;

    /* keep last four terms for the recurrence */
    seq_t prev4 = a1; /* a(n-4) */
    seq_t prev3 = a2; /* a(n-3) */
    seq_t prev2 = a3; /* a(n-2) */
    seq_t prev1 = a4; /* a(n-1) */
    seq_t current;

    for (idx_t i = 5U; i <= n; ++i) {
        current = 11 * prev1 - 11 * prev2 + 11 * prev3 - 10 * prev4;
        /* shift the window */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}
