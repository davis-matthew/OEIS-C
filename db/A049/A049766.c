/*
 * A049766  a(n) = Sum_{k=1..n} T(n,k), array T as in A049765.
 * https://oeis.org/A049766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} ((n mod k)+(k mod n))
 * Range:     u8 n <= 20, u16 n <= 312, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 1 the sum is zero, matching the sequence offset. */
    if (n <= 1) return 0;

    seq_t seq_n = (seq_t)n;
    seq_t sum_rem = 0;

    for (idx_t k = 1; k < n; ++k) {
        seq_t kk = (seq_t)k;
        sum_rem += seq_n % kk;
    }

    seq_t triangular = seq_n * (seq_n - (seq_t)1) / (seq_t)2;
    return triangular + sum_rem;
}
