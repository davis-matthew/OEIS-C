/*
 * A076482  Triangle with T(n,k)=n!*(k-1)^k/k! where 1<=k<=n.
 * https://oeis.org/A076482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r and k are chosen such that n is the nth
 *            element of the triangular array read row-wise. T(r,k) = r! *
 *            (k-1)^k / k! (1 <= k <= r)
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 62, u64 n <= 150,
 *            u128 n <= 402, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of sequence A076482 (row‑major order). */
seq_t a(idx_t n)
{
    /* n >= 1 is the offset of the sequence.  Behaviour for n < 1 is
       unspecified. */
    if (n < 1) return 0;

    /* Find the row r such that C(r-1) < n <= C(r) where C(r) = r(r+1)/2. */
    idx_t r = 1;
    idx_t cumulative = 1;          /* C(r) */
    while (n > cumulative) {
        ++r;
        cumulative += r;
    }

    /* Column k is the position within the row. */
    idx_t prevSum = (r - 1) * r / 2; /* C(r-1) */
    idx_t k = n - prevSum;           /* 1 <= k <= r */

    /* Compute P = r! / k! = product_{i=k+1}^{r} i. */
    seq_t result = 1;
    for (idx_t i = k + 1; i <= r; ++i) {
        result *= (seq_t)i;
    }

    /* Compute (k-1)^k. */
    seq_t powv = 1;
    seq_t base = (k > 0) ? (seq_t)(k - 1) : 0; /* For k==1, base = 0. */
    for (idx_t i = 0; i < k; ++i) {
        powv *= base;
    }

    result *= powv;
    return result;
}
