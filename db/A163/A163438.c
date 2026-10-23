/*
 * A163438  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=13, a(2)=156, a(3)=1872, a(4)=22464, a(5)=269490.
 *            For n≥6: a(n)=11*a(n−1)+11*a(n−2)+11*a(n−3)+11*a(n−4)−66*a(n−5).
 *            (The recurrence follows from the rational generating function on
 *            OEIS.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small indices handled directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)156;
    if (n == 3) return (seq_t)1872;
    if (n == 4) return (seq_t)22464;
    if (n == 5) return (seq_t)269490;

    /* Working window holding the last five terms
       v[0] = a(k-5), v[1] = a(k-4), …, v[4] = a(k-1) for current k */
    seq_t v[5] = { (seq_t)13,   /* a(1) */
                   (seq_t)156,  /* a(2) */
                   (seq_t)1872, /* a(3) */
                   (seq_t)22464,/* a(4) */
                   (seq_t)269490 }; /* a(5) */

    for (idx_t k = 6; k <= n; ++k) {
        seq_t val = (seq_t)11 * v[4] + (seq_t)11 * v[3] +
                    (seq_t)11 * v[2] + (seq_t)11 * v[1] -
                    (seq_t)66 * v[0];

        /* Shift window: drop oldest, make room for new value */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = val;
    }
    return v[4];
}
