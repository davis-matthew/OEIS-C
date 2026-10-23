/*
 * A167713  a(n) = 16^n * Sum_{k=0..n} binomial(2*k, k) / 16^k.
 * https://oeis.org/A167713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^n * Sum_{k=0..n} binomial(2*k, k) / 16^k
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function uses the simple recurrence
     a(0) = 1
     a(n) = 16 * a(n-1) + C(2*n, n)   for n ≥ 1
   The binomial coefficients are generated with the multiplicative
   recurrence
     C(0,0) = 1
     C(2*n, n) = C(2*(n-1), n-1) * (4*n - 2) / n
   which uses only integer arithmetic and fits the required
   constraints.  The implementation runs in O(n) time and uses
   only operations defined on seq_t. */

seq_t a(idx_t n)
{
    seq_t res = 1;   /* a(0) */
    seq_t bin = 1;   /* C(0,0) */

    for (idx_t i = 1; i <= n; ++i) {
        /* Update binomial coefficient C(2*i, i) */
        seq_t mult = (seq_t)(4 * i - 2);      /* 4*i-2 fits in idx_t */
        bin = bin * mult / i;                 /* exact division */

        /* a(i) = 16*a(i-1) + C(2*i,i) */
        res = res * 16 + bin;
    }

    return res;
}
