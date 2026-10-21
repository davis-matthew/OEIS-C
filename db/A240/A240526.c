/*
 * A240526  a(n) = 2^(n-2)*(2^(n+4)-(-1)^n+13).
 * https://oeis.org/A240526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2)*(2^(n+4)-(-1)^n+13)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (used for all n): a(0)=7, a(1)=23, a(2)=76,
   a(n)=4*a(n-1)+4*a(n-2)-16*a(n-3) for n>2. */

seq_t a(idx_t n) {
    /* Handle initial values directly. */
    if (n == 0) return (seq_t)7;
    if (n == 1) return (seq_t)23;
    if (n == 2) return (seq_t)76;

    /* Use only three variables to store last three values. */
    seq_t a0 = (seq_t)7;   /* a(n-3) */
    seq_t a1 = (seq_t)23;  /* a(n-2) */
    seq_t a2 = (seq_t)76;  /* a(n-1) */
    seq_t val;

    for (idx_t i = 3; i <= n; ++i) {
        /* val = 4*a(n-1) + 4*a(n-2) - 16*a(n-3) */
        val = (seq_t)4 * a2 + (seq_t)4 * a1 - (seq_t)16 * a0;
        /* Shift the window of last three values. */
        a0 = a1;
        a1 = a2;
        a2 = val;
    }
    return a2;
}
