/*
 * A116837  Number of permutations of length n which avoid the patterns 3421,
 *          4123, 4312; or avoid the patterns 2341, 3142, 3214.
 * https://oeis.org/A116837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies a(1)=1, a(2)=2, a(3)=6, a(4)=21 for n≥5
 *            a(n)=5·a(n-1)-7·a(n-2)+6·a(n-3)-2·a(n-4)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A116837.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* base values: a(0)=0 (unused), a(1)=1, a(2)=2, a(3)=6, a(4)=21 */
    static const seq_t init[5] = {0, 1, 2, 6, 21};

    if (n < 5)
        return init[n];

    /* a_nm1 = a(n‑1), a_nm2 = a(n‑2), a_nm3 = a(n‑3), a_nm4 = a(n‑4) */
    seq_t a_nm1 = 21;   /* a(4) */
    seq_t a_nm2 = 6;    /* a(3) */
    seq_t a_nm3 = 2;    /* a(2) */
    seq_t a_nm4 = 1;    /* a(1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_val = 5 * a_nm1 + 6 * a_nm3;          /* 5·a(n‑1)+6·a(n‑3) */
        new_val -= (7 * a_nm2 + 2 * a_nm4);            /* subtract (7·a(n‑2)+2·a(n‑4)) */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = new_val;
    }
    return a_nm1;
}
