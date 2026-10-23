/*
 * A048906  Octagonal heptagonal numbers.
 * https://oeis.org/A048906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 232323*a(n-1) - 232323*a(n-2) + a(n-3)
 * with initial values: a(1)=1, a(2)=297045, a(3)=69010153345
 * (Valid for all n ≥ 1; the recurrence applies from n = 4 onward.)
 */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)297045;
    if (n == 3)
        return (seq_t)69010153345ULL;

    /* iterate the linear recurrence */
    seq_t a_nm3 = (seq_t)1;                /* a(n-3) */
    seq_t a_nm2 = (seq_t)297045;           /* a(n-2) */
    seq_t a_nm1 = (seq_t)69010153345ULL;   /* a(n-1) */
    seq_t a_n    = 0;                      /* current term */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)232323 * a_nm1
               - (seq_t)232323 * a_nm2
               + a_nm3;
        /* shift the window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
