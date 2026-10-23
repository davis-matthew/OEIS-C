/*
 * A115881  a(n) is the largest positive y satisfying the Diophantine equation
 *          x^2=y(y+n). a(n)=0 if there are no solutions.
 * https://oeis.org/A115881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   piecewise by n mod 4 * a(4*j) = j^2 - 2*j + 1 * a(4*j+1) = 4*j^2
 *            * a(4*j+2) = 2*j^2 * a(4*j+3) = 4*j^2 + 4*j + 1 * (j =
 *            floor(n/4) for the appropriate residue)
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 4;
    idx_t j = (n - r) / 4;          /* integer division gives correct j */
    seq_t j_sq = (seq_t)j * (seq_t)j;
    seq_t res;

    if (r == 0) {
        res = j_sq - (seq_t)2 * (seq_t)j + (seq_t)1;
    } else if (r == 1) {
        res = (seq_t)4 * j_sq;
    } else if (r == 2) {
        res = (seq_t)2 * j_sq;
    } else { /* r == 3 */
        res = (seq_t)4 * j_sq + (seq_t)4 * (seq_t)j + (seq_t)1;
    }

    return res;
}
