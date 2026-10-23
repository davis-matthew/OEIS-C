/*
 * A024420  a(n) = n! * Sum_{j=0..floor(n/2)} (-1)^j/binomial(n,j).
 * https://oeis.org/A024420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{j=0..floor(n/2)} (-1)^j / binomial(n,j)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n!  (term holds j!*(n-j)! for current j) */
    seq_t term = 1;
    for (idx_t i = 1; i <= n; ++i) {
        term *= (seq_t)i;
    }

    /* sum = term for j = 0 (positive) */
    seq_t sum = term;

    idx_t k = n / 2;            /* floor(n/2) */
    for (idx_t j = 0; j < k; ++j) {
        /* term_{j+1} = (j+1) * term_j / (n - j) */
        seq_t next = term * (seq_t)(j + 1);
        next /= (seq_t)(n - j);

        /* add or subtract based on sign of (-1)^(j+1) */
        if (((j + 1) & 1) != 0) {
            sum -= next;       /* negative contribution */
        } else {
            sum += next;       /* positive contribution */
        }

        term = next;
    }

    return sum;
}
