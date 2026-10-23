/*
 * A163091  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) + 13*a(n-2) + 13*a(n-3) - 91*a(n-4) for n ≥ 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=15, a(2)=210, a(3)=2940, a(4)=41055*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)210;
    if (n == 3) return (seq_t)2940;
    if (n == 4) return (seq_t)41055;

    seq_t vals[5];
    vals[0] = (seq_t)1;        /* a0 */
    vals[1] = (seq_t)15;       /* a1 */
    vals[2] = (seq_t)210;      /* a2 */
    vals[3] = (seq_t)2940;     /* a3 */
    vals[4] = (seq_t)41055;    /* a4 */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)13 * vals[(i - 1) % 5] +
                     (seq_t)13 * vals[(i - 2) % 5] +
                     (seq_t)13 * vals[(i - 3) % 5] -
                     (seq_t)91 * vals[(i - 4) % 5];
        vals[i % 5] = next;
    }
    return vals[n % 5];
}
