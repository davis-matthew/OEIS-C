/*
 * A133271  Indices of 7-gonal numbers which are also centered 7-gonal
 *          numbers.
 * https://oeis.org/A133271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 13*a(n-2) + a(n-3) for n ≥ 4 with initial
 *            values: a(1)=1, a(2)=8, a(3)=92
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)92;

    /* Iteratively compute the sequence using the recurrence. */
    seq_t a_nm3 = (seq_t)1;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)8;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)92;  /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        a_n = (seq_t)13 * a_nm1 - (seq_t)13 * a_nm2 + a_nm3;
        /* Shift the window for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
