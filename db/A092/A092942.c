/*
 * A092942  A Fibonacci sequence with "corrections" at every third step:
 *          -++-++-++-++-++..., i.e., at every 3rd step there is a subtraction
 *          instead of an addition.
 * https://oeis.org/A092942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1; for n>=2, a(n) = a(n-1) + a(n-2) if n % 3 != 2
 *            a(n) = a(n-1) - a(n-2) if n % 3 == 2
 * Range:     u8 n <= 383, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a_nm2 = 0; /* a(n-2) */
    seq_t a_nm1 = 1; /* a(n-1) */
    seq_t a_n   = 0; /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        if (i % 3 == 2)
            a_n = a_nm1 - a_nm2;   /* subtraction step */
        else
            a_n = a_nm1 + a_nm2;   /* addition step */

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
