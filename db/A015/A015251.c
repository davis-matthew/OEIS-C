/*
 * A015251  Gaussian binomial coefficient [ n,2 ] for q = -3.
 * https://oeis.org/A015251
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence:  A015251 – Gaussian binomial coefficient [n,2] for q = -3.
 * Recurrence (valid for n ≥ 3)
 *   a(2) = 1
 *   a(3) = 7
 *   a(4) = 70
 *   a(n) = 7*a(n-1) + 21*a(n-2) – 27*a(n-3)   (n ≥ 5)
 *
 * The recurrence produces exactly the OEIS values and keeps all intermediate
 * operations within the range of seq_t for all terms that fit in seq_t.
 */

seq_t a(idx_t n)
{
    /* Base values – defined for the offset of the sequence (n ≥ 2). */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)70;

    /* Iterate the recurrence up to the requested index. */
    seq_t a_nm3 = (seq_t)1;   /* a(n-3)   */
    seq_t a_nm2 = (seq_t)7;   /* a(n-2)   */
    seq_t a_nm1 = (seq_t)70;  /* a(n-1)   */
    seq_t a_next;

    for (idx_t k = 5; k <= n; ++k)
    {
        /* a(k) = 7*a(k-1) + 21*a(k-2) – 27*a(k-3) */
        a_next = 7 * a_nm1
               + 21 * a_nm2
               - 27 * a_nm3;

        /* Shift the windows for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_next;
    }

    return a_nm1;  /* a(n) is stored in a_nm1 after the loop ends */
}
