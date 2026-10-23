/*
 * A115792  a(n) = ceiling(g(A000073(n))) with g(k) = (k-1)^2/(4k).
 * https://oeis.org/A115792
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(((A000073(n)-1)^2)/(4*A000073(n))) (offset 2)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the OEIS offset is not defined – return 0 safely. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Compute A000073(n), the Tribonacci number at index n. */
    seq_t trib;
    if (n == 0 || n == 1) {
        trib = (seq_t)0;
    } else if (n == 2 || n == 3) {
        trib = (seq_t)1;
    } else {
        /* Iteratively compute trib(n) using only O(1) space. */
        seq_t t0 = (seq_t)0;              /* trib(i-3) */
        seq_t t1 = (seq_t)0;              /* trib(i-2) */
        seq_t t2 = (seq_t)1;              /* trib(i-1) */
        seq_t t3;
        for (idx_t i = 3; i <= n; ++i) {
            t3 = t0 + t1 + t2;            /* trib(i)  */
            t0 = t1;
            t1 = t2;
            t2 = t3;
        }
        trib = t2;
    }

    /* Evaluate g(k) = (k-1)^2 / (4k) and take the ceiling. */
    seq_t k = trib;
    seq_t k_minus = k - (seq_t)1;
    seq_t numerator = k_minus * k_minus;
    seq_t denom = (seq_t)4 * k;

    /* Ceil division: (numerator + denom - 1) / denom. */
    seq_t result = (numerator + denom - (seq_t)1) / denom;
    return result;
}
