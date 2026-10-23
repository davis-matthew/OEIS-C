/*
 * A167613  Array T(n,k) read by antidiagonals: the k-th term of the n-th
 *          difference of A131531.
 * https://oeis.org/A167613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   a(n) = T(i, k) where i = n - s*(s+1)/2 s = smallest integer such
 *            that (s+1)*(s+2)/2 > n k = s - i T(n,k) = sum_{j=0}^{n}
 *            (-1)^{n-j} * C(n,j) * A131531(k+j) A131531(m) is 0,0,1,0,0,-1
 *            repeating, i.e. A131531(m) = {0,0,1,0,0,-1}[m mod 6]
 * Range:     i8 n <= 34, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find diagonal s */
    idx_t s = 0;
    while (1) {
        unsigned __int128 t = (unsigned __int128)(s + 1) * (s + 2) / 2;
        if (t > n) break;
        ++s;
    }

    unsigned __int128 tri = (unsigned __int128)s * (s + 1) / 2;
    idx_t i = n - (idx_t)tri;          /* row index in T */
    idx_t k = s - i;                   /* column index in T */

    /* compute T(i,k) */
    static const int base[6] = {0, 0, 1, 0, 0, -1};
    seq_t result = 0;
    seq_t binom = 1;                    /* C(i,0) */
    int sign = (i & 1) ? -1 : 1;         /* (-1)^i for j=0 */

    for (idx_t j = 0; j <= i; ++j) {
        int add = base[(k + j) % 6];
        seq_t term = binom * (seq_t)add;
        if (sign > 0)
            result += term;
        else
            result -= term;

        /* update binomial coefficient: C(i, j+1) = C(i, j) * (i-j)/(j+1) */
        if (j < i) {
            binom = binom * (seq_t)(i - j);
            binom = binom / (seq_t)(j + 1);
        }
        sign = -sign;                   /* alternate sign each step */
    }

    return result;
}
