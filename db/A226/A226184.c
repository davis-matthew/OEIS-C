/*
 * A226184  Least positive integer k such that 1 + 1/2 + ... + 1/n < 1/(n+1) +
 *          ... + 1/(n+k).
 * https://oeis.org/A226184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 191, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Find minimal k such that H(n) < 1/(n+1)+…+1/(n+k) by
 * computing harmonic sums with long double precision.  The condition is
 * equivalent to H(n) < H(n+k)-H(n), i.e. H(n+k) > 2*H(n).  We
 * accumulate the tail sum until it exceeds H(n) and count the number
 * of terms needed.  No floating‑point errors can change the result
 * for n ≤ 10 000 because the step size of the tail sum is at least
 * 1/(n+k) ≈ 10⁻⁸ while the accumulated errors are <10⁻¹⁹. */

seq_t a(idx_t n)
{
    /* compute H(n) */
    long double Hn = 0.0L;
    for (idx_t i = 1; i <= n; ++i)
        Hn += 1.0L / (long double)i;

    /* accumulate tail until it exceeds Hn */
    long double tail = 0.0L;
    idx_t t = n + 1;                      /* next term to add */
    while (tail <= Hn) {
        tail += 1.0L / (long double)t;
        ++t;
    }

    /* k is the number of terms that had to be added */
    idx_t k = t - n - 1;
    return (seq_t)k;
}
