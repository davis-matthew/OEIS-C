/*
 * A219499  Number of n X 5 arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..1 n X 5 array.
 * https://oeis.org/A219499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n>7, with initial values a(1)=5, a(2)=9, a(3)=26, a(4)=58,
 *            a(5)=107, a(6)=179, a(7)=281
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)26;
    if (n == 4) return (seq_t)58;
    if (n == 5) return (seq_t)107;
    if (n == 6) return (seq_t)179;
    if (n == 7) return (seq_t)281;

    /* Use the recurrence for n >= 8.  We keep the last five values
       and iterate until the requested n.  Signed intermediate
       computation with __int128 guarantees correctness without
       overflow for all n up to at least 10 000. */
    seq_t m1 = (seq_t)26;   /* a(3)  */
    seq_t m2 = (seq_t)58;   /* a(4)  */
    seq_t m3 = (seq_t)107;  /* a(5)  */
    seq_t m4 = (seq_t)179;  /* a(6)  */
    seq_t m5 = (seq_t)281;  /* a(7)  */
    seq_t val = 0;

    for (idx_t i = 8; i <= n; ++i) {
        /* Convert to signed 128‑bit values for safe intermediate
           arithmetic; all final results fit comfortably in that
           range for n <= 10 000. */
        __int128 v1 = (__int128)(unsigned long long)m5;
        __int128 v2 = (__int128)(unsigned long long)m4;
        __int128 v3 = (__int128)(unsigned long long)m3;
        __int128 v4 = (__int128)(unsigned long long)m2;
        __int128 v0 = (__int128)(unsigned long long)m1;

        __int128 intermediate =
              5 * v1
            - 10 * v2
            + 10 * v3
            - 5  * v4
            + v0;

        val = (seq_t)intermediate;   /* convert back to seq_t */

        /* Slide the window of the last five values. */
        m1 = m2;
        m2 = m3;
        m3 = m4;
        m4 = m5;
        m5 = val;
    }

    return val;
}
