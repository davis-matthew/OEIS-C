/*
 * A079986  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={-2,0,2}.
 * https://oeis.org/A079986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is odd; otherwise let n = 2m. Define tribonacci
 *            T_0 = 1, T_1 = 1, T_2 = 2, T_m = T_{m-1} + T_{m-2} + T_{m-3} for
 *            m ≥ 3. Then a(n) = T_m^2
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 37, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Even indices only */
    if (n & 1) {
        return 0;
    }

    /* Work with the half-index m = n/2 */
    idx_t m = n >> 1;

    /* Handle small cases directly */
    if (m == 0)          return 1;        /* T_0 = 1, 1^2 = 1 */
    if (m == 1)          return 1;        /* T_1 = 1, 1^2 = 1 */
    if (m == 2)          return 4;        /* T_2 = 2, 2^2 = 4 */

    /* Iterate the tribonacci recurrence */
    seq_t t0 = 1;  /* T_{i-3} */
    seq_t t1 = 1;  /* T_{i-2} */
    seq_t t2 = 2;  /* T_{i-1} */
    for (idx_t i = 3; i <= m; ++i) {
        seq_t t_next = t2 + t1 + t0;
        t0 = t1;
        t1 = t2;
        t2 = t_next;
    }

    /* Result is T_m squared */
    return t2 * t2;
}
