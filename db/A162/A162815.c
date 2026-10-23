/*
 * A162815  a(n) = 8*a(n-1)-13*a(n-2) for n > 1; a(0) = 5, a(1) = 23.
 * https://oeis.org/A162815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1) - 13*a(n-2) for n>1;
   initial values: a(0) = 5, a(1) = 23 */

/* Compute the n‑th term of A162815.
 * No dynamic memory allocation is required, so the code
 * compiles cleanly in both C11 and C++17 where `seq_t`
 * may be a built‑in integral type or an arbitrary‑precision
 * integer class. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 5;
    if (n == 1)
        return 23;

    seq_t a_nm2 = 5;   /* a(n-2)  */
    seq_t a_nm1 = 23;  /* a(n-1)  */
    seq_t a_n   = 0;   /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        a_n = 8 * a_nm1 - 13 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
