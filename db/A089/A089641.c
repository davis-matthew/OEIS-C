/*
 * A089641  Number of k, 1<=k<=n, such that the number of elements in the
 *          continued fraction for n/k is maximum.
 * https://oeis.org/A089641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of k, 1 ≤ k ≤ n, such that the length of the
 *  simple continued fraction of the rational n/k is maximal among all
 *  such k.  The length of a continued fraction for a > 0 is the
 *  number of quotients obtained by the Euclidean algorithm.           */

seq_t a(idx_t n) {
    seq_t max_len = 0;          /* longest continued‑fraction length seen */
    seq_t count   = 0;          /* how many k achieve that length */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t a_rem = n;        /* numerator */
        idx_t b_rem = k;        /* denominator */
        seq_t len = 0;          /* length of this continued fraction */

        /* Euclidean algorithm counting the quotients */
        while (b_rem) {
            ++len;
            idx_t r = a_rem % b_rem;
            a_rem = b_rem;
            b_rem = r;
        }

        if (len > max_len) {
            max_len = len;
            count   = 1;
        } else if (len == max_len) {
            ++count;
        }
    }
    return count;
}
