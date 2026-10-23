/*
 * A015477  q-Fibonacci numbers for q=6, scaling a(n-1).
 * https://oeis.org/A015477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula:
   a(0) = 0
   a(1) = 1
   a(n) = 6^(n-1) * a(n-1) + a(n-2)   for n ≥ 2
   The function computes the term using only +, *, and the seq_t type. */
seq_t a(idx_t n)
{
    /* Handle the trivial initial cases immediately. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* a_nm2 = a(n-2), a_nm1 = a(n-1) */
    seq_t a_nm2 = (seq_t)0;
    seq_t a_nm1 = (seq_t)1;
    /* pow6 holds 6^(i-1) while we compute a(i). Initially for i = 2, pow6 = 6^1 = 6. */
    seq_t pow6   = (seq_t)6;

    /* Iteratively compute a(2) .. a(n). */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term = a_nm1 * pow6 + a_nm2;  /* a(i) = 6^(i-1)*a(i-1) + a(i-2) */
        a_nm2 = a_nm1;                      /* shift for next step */
        a_nm1 = term;
        pow6 = pow6 * (seq_t)6;             /* prepare 6^(i) for next iteration */
    }

    return a_nm1;
}
