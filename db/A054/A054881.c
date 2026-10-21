/*
 * A054881  Number of walks of length n along the edges of an octahedron
 *          starting and ending at a vertex and also ( with a(0)=0 ) between
 *          two opposite vertices.
 * https://oeis.org/A054881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = (4^n + (-1)^n*2^(n+1) + 3*0^n)/6, n ≥ 0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values from the definition */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;

    /* Iterative computation using the linear recurrence
       a(n) = 2*a(n-1) + 8*a(n-2)    for n ≥ 3   */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)0;   /* a(1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        if (i == 2)
            a_n = (seq_t)4;
        else
            a_n = (seq_t)2 * a_nm1 + (seq_t)8 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
