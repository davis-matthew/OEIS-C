/*
 * A101166  a(n) = (15*n^4 + 22*n^3 + 45*n^2 + 14*n) / 24.
 * https://oeis.org/A101166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*n^4 + 22*n^3 + 45*n^2 + 14*n) / 24
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 287, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Direct computation using the polynomial formula. */
    if (n < 5) {
        switch (n) {
            case 0: return (seq_t)0;
            case 1: return (seq_t)4;
            case 2: return (seq_t)26;
            case 3: return (seq_t)94;
            case 4: return (seq_t)251;
            default: return (seq_t)0; /* unreachable */
        }
    }

    /* Compute using the recurrence for O(n) speed and to keep
       intermediate values small.  The recurrence is:
           a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5)
       with the above initial values. */
    seq_t a0 = (seq_t)0;    /* a(n-5) */
    seq_t a1 = (seq_t)4;    /* a(n-4) */
    seq_t a2 = (seq_t)26;   /* a(n-3) */
    seq_t a3 = (seq_t)94;   /* a(n-2) */
    seq_t a4 = (seq_t)251;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t ai = a0 + (seq_t)5 * a4 - (seq_t)10 * a3 + (seq_t)10 * a2 - (seq_t)5 * a1;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;
}
