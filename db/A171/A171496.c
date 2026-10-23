/*
 * A171496  a(n) = 6*a(n-1) - 8*a(n-2) for n > 1; a(0) = 6, a(1) = 28.
 * https://oeis.org/A171496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 8*a(n-2) for n > 1 Initial values: a(0) = 6,
 *            a(1) = 28
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 6;
    if (n == 1) return 28;

    seq_t a_nm2 = 6;   /* a(n-2) */
    seq_t a_nm1 = 28;  /* a(n-1) */
    seq_t a_n = 0;     /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* Perform the recurrence.  The intermediate
           multiplication is done in the type seq_t.
           For built‑in unsigned types this is safe for all
           n whose results fit in seq_t.  For arbitrary
           precision types, the class implements correct
           arithmetic without overflow. */
        a_n = a_nm1 * 6 - a_nm2 * 8;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
