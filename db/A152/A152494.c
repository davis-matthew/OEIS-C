/*
 * A152494  1/3 of the number of permutations of 2 indistinguishable copies of
 *          1..n with exactly 2 local maxima.
 * https://oeis.org/A152494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (13*10^(n-1) - 13*3^(n-1) - 14*(n-1)*3^(n-1))/49
 *            Recurrence: a(1)=0, a(2)=1, a(3)=19,
 *            a(n)=16*a(n-1)-69*a(n-2)+90*a(n-3) for n>3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for n<1 */

    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 19;

    /* iterate using the linear recurrence */
    seq_t a_nm3 = 0;   /* a(1)  */
    seq_t a_nm2 = 1;   /* a(2)  */
    seq_t a_nm1 = 19;  /* a(3)  */
    seq_t curr = 0;

    for (idx_t i = 4; i <= n; ++i) {
        curr = 16 * a_nm1;          /* 16*a(n-1)          */
        curr += 90 * a_nm3;         /* + 90*a(n-3)        */
        curr -= 69 * a_nm2;         /* - 69*a(n-2)        */

        /* shift the last three values */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }

    return curr;   /* for n>=4 */
}
