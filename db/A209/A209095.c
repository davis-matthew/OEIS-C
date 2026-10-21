/*
 * A209095  Number of n X 3 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A209095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5, a(2)=76, a(3)=1326, a(4)=23248,
   a(n)=20*a(n-1)-44*a(n-2)+16*a(n-3) for n>4 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;  /* safety for out‑of‑range use */
    }

    /* base cases */
    switch (n) {
        case 1: return (seq_t)5;
        case 2: return (seq_t)76;
        case 3: return (seq_t)1326;
        case 4: return (seq_t)23248;
    }

    /* initialise with a(2), a(3), a(4) so that the recurrence can
       compute a(5) on the first loop iteration. */
    seq_t a_nm3 = (seq_t)76;       /* a(n-3) for the next term   */
    seq_t a_nm2 = (seq_t)1326;     /* a(n-2)                    */
    seq_t a_nm1 = (seq_t)23248;    /* a(n-1)                    */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 5; i <= n; ++i) {
        /* compute a(n) = 20*a(n-1) - 44*a(n-2) + 16*a(n-3)   */
        a_n = ((seq_t)20 * a_nm1) + ((seq_t)16 * a_nm3)
             - ((seq_t)44 * a_nm2);

        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
