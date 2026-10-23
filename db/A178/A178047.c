/*
 * A178047  Consider the Farey tree A049455/A049456; a(n) = row at which the
 *          denominator n first appears (assumes first row is labeled row 0).
 * https://oeis.org/A178047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{1 ≤ k < n, gcd(n,k)=1} (sum_of_quotients(n,k) – 1) *
 *            where sum_of_quotients(n,k) is the sum of the quotients that
 *            appear in the Euclidean algorithm * applied to (n,k)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)0;
    }
    seq_t N = (seq_t)n;
    seq_t min_cost = N;          /* cost for k = 1 */
    for (idx_t k = 1; k < n; ++k) {
        seq_t a_den = N;
        seq_t b_den = k;
        seq_t sum = (seq_t)0;
        while (b_den != (seq_t)0) {
            seq_t q = a_den / b_den;
            sum += q;
            seq_t r = a_den % b_den;
            a_den = b_den;
            b_den = r;
        }
        if (a_den == ((seq_t)1) && sum < min_cost) {
            min_cost = sum;
        }
    }
    return min_cost - ((seq_t)1);
}
