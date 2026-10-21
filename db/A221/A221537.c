/*
 * A221537  Number of 0..3 arrays of length n with each element differing from
 *          at least one neighbor by something other than 1, starting with 0.
 * https://oeis.org/A221537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - a(n-3) + a(n-4) Initial values:
 *            a(1)=0, a(2)=3, a(3)=8, a(4)=30
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[4] = {0, 3, 8, 30};

    if (n == 0) return 0; /* Undefined for n<1, but return 0 */
    if (n <= 4) return base[n - 1];

    seq_t a_nm4 = base[0]; /* a(n-4) */
    seq_t a_nm3 = base[1]; /* a(n-3) */
    seq_t a_nm2 = base[2]; /* a(n-2) */
    seq_t a_nm1 = base[3]; /* a(n-1) */
    seq_t curr = 0;

    for (idx_t i = 5; i <= n; ++i) {
        curr = 3 * a_nm1 + 2 * a_nm2 - a_nm3 + a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }

    return curr;
}
