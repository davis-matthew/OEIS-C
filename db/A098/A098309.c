/*
 * A098309  Unsigned member r = -10 of the family of Chebyshev sequences
 *          S_r(n) defined in A092184.
 * https://oeis.org/A098309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - a(n-2) + 2*(-1)^(n+1) Sequence A098309:
 *            a(0)=0, a(1)=1, a(2)=10
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: no memoisation or dynamic allocation used – the
   recurrence can be computed in O(n) time per call with
   only a few temporaries. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 10;

    /* a(1) and a(2) for the recurrence */
    seq_t a_prev  = 1;   /* a(1) */
    seq_t a_curr  = 10;  /* a(2) */
    seq_t a_next;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 12*a_curr - a_prev + 2*(-1)^(i+1) */
        a_next = 12 * a_curr - a_prev;
        /* (-1)^(i+1) is +1 if i is odd, -1 if i is even */
        if ((i & 1ULL) == 0)      /* i even → (-1)^(i+1) = -1 */
            a_next -= 2;
        else                      /* i odd  → (+1)          */
            a_next += 2;
        /* roll the values for the next iteration */
        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;   /* a(n) */
}
