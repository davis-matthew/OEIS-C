/*
 * A096007  Triangle read by rows: T(n,k) = smallest prime factor of
 *          binomial(n,k), 1 <= k <= n-1.
 * https://oeis.org/A096007
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = smallest prime factor of binomial(r,k), where the n‑th
 *            term is the (n‑1)-th element of the triangle with rows starting
 *            at r = 2 and columns k = 1 … r‑1
 * Range:     u8 n <= 1276, u16 n <= 1276, u32 n <= 1276, u64 n <= 1276,
 *            u128 n <= 1276, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small prime list up to 200 (enough for r ≤ 142). */
static const uint16_t primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
    53,59,61,67,71,73,79,83,89,97,101,103,107,
    109,113,127,131,137,139
};
static const int primes_count = sizeof(primes) / sizeof(primes[0]);

/* Legendre exponent of prime p in n! : e_p(n!). */
static inline idx_t exp_in_fact(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* Find the smallest prime factor of C(r,k). */
static seq_t spf_binom(idx_t r, idx_t k)
{
    /* r is at most about 142, so we iterate over the small prime list. */
    for (int i = 0; i < primes_count; ++i) {
        idx_t p = primes[i];
        if (p > r) break;           /* prime larger than r cannot divide C(r,k) */
        idx_t e = exp_in_fact(r, p);
        e -= exp_in_fact(k, p);
        e -= exp_in_fact(r - k, p);
        if (e > 0)
            return (seq_t)p;
    }
    /* In theory this point is unreachable for 1 ≤ k ≤ r-1. */
    return (seq_t)0;
}

/* Main function for the OEIS compiler. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined below the offset */

    /* Convert to zero–based index in the triangle sequence. */
    idx_t m = n - 2;

    /* Find the row r such that (r-1)*r/2 > m. */
    idx_t r = 2;
    while (((unsigned __int128)(r - 1) * r) / 2 <= m)
        ++r;

    /* Compute the previous cumulative count. */
    idx_t prev = ((unsigned __int128)(r - 2) * (r - 1)) / 2;

    /* Column k (1‑based). */
    idx_t k = m - prev + 1;

    /* Return the smallest prime factor of C(r,k). */
    return spf_binom(r, k);
}
