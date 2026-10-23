/*
 * A127569  Triangle read by rows: product of the Mobius matrix A054525 by the
 *          diagonal matrix diag(A000203(n)).
 * https://oeis.org/A127569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = μ(r/k) * σ(k) where r = the unique integer with (r-1)*r/2
 *            < n <= r*(r+1)/2, k = n - r*(r-1)/2 (1 ≤ k ≤ r). μ(m) – Mobius
 *            function, σ(k) – sum of divisors of k
 * Range:     i8 n <= 115, i16 n <= 115, i32 n <= 115, i64 n <= 115,
 *            i128 n <= 115, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the Möbius function μ(m). */
static seq_t mobius(idx_t m)
{
    if (m == 1) return 1;
    seq_t mu = 1;
    idx_t t = m;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            t /= p;
            if (t % p == 0) return 0;
            mu = -mu;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) mu = -mu;
    return mu;
}

/* Return the sum of divisors σ(n). */
static seq_t sigma(idx_t n)
{
    seq_t result = 1;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            seq_t term = 1;
            seq_t p_pow = 1;
            while (t % p == 0) {
                t /= p;
                p_pow *= p;
                term += p_pow;
            }
            result *= term;
        }
    }
    if (t > 1) {
        result *= (1 + (seq_t)t);
    }
    return result;
}

/* Compute the n-th term of A127569. */
seq_t a(idx_t n)
{
    /* Find row r: smallest r with r*(r+1)/2 >= n. */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;

    /* Column within the row. */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev;                /* 1 ≤ k ≤ r */

    if (r % k != 0) return 0;          /* Entry is zero when k ∤ r */

    idx_t div = r / k;
    seq_t mu_val = mobius(div);
    if (mu_val == 0) return 0;

    seq_t sigma_val = sigma(k);
    return mu_val * sigma_val;
}
