/*
 * A204000  Symmetric matrix based on f(i,j)=min{i(j+1)-1,j(i+1)-1}, by
 *          antidiagonals.
 * https://oeis.org/A204000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = min{ i*(j+1)-1, j*(i+1)-1 } where (i,j) are the indices
 *            of * the symmetric matrix M(i,j)=min{i(j+1)-1,j(i+1)-1} read by
 *            anti‑diagonals; * i+j is the minimal s≥2 such that (s−1)*s/2 ≥
 *            n. * i = (n−(s−2)(s−1)/2) and j = s−i
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the diagonal s with (s-1)*s/2 ≥ n */
    idx_t s = 2;
    unsigned __int128 sum = 1;      /* T(s) */
    unsigned __int128 prev = 0;     /* T(s-1) */

    while (sum < (unsigned __int128)n) {
        s++;
        prev = sum;
        sum += (unsigned __int128)(s - 1);
    }

    /* index inside the diagonal (0‑based) */
    idx_t t = (idx_t)(n - (idx_t)prev - 1);

    /* map to matrix coordinates */
    idx_t i = t + 1;
    idx_t j = s - i;

    /* compute the matrix entry */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t val1 = ii * (jj + (seq_t)1) - (seq_t)1;
    seq_t val2 = jj * (ii + (seq_t)1) - (seq_t)1;

    return val1 < val2 ? val1 : val2;
}
