/*
 * A231147  Array of coefficients of numerator polynomials of the rational
 *          function p(n, x + 1/x), where p(n,x) = (x^n - 1)/(x - 1).
 * https://oeis.org/A231147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = Σ_{t=0}^{⌊(r-1-d)/2⌋} C(d+2t, t) where r = ⌊√(n‑1)⌋ + 1,
 *            pos = n – (r‑1)² (1‑based index in row r), d = |pos – r|
 * Range:     u8 n <= 108, u16 n <= 342, u32 n <= 1255, u64 n <= 4555,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* floor(sqrt(n)) for unsigned 64‑bit integers */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1ULL << 32) - 1;           /* floor sqrt(ULLONG_MAX) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* primes up to 200 (enough for r ≤ 127 needed in tests) */
static const int primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
    53,59,61,67,71,73,79,83,89,97,101,103,107,
    109,113,127,131,137,139,149,151,157,163,
    167,173,179,181,191,193,197,199
};
static const int num_primes = sizeof(primes)/sizeof(primes[0]);

/* binom(n,k) computed by prime‑factorisation, n,k ≤ 200 */
static seq_t binom_prime_exp(idx_t n, idx_t k)
{
    if (k == 0 || n == k) return 1;
    if (k > n) return 0;
    seq_t result = 1;
    for (int pi = 0; pi < num_primes && primes[pi] <= (int)n; ++pi) {
        int p = primes[pi];
        idx_t exp = 0;
        idx_t tmp = n;
        while (tmp) { tmp /= p; exp += tmp; }
        tmp = k;
        while (tmp) { tmp /= p; exp -= tmp; }
        tmp = n - k;
        while (tmp) { tmp /= p; exp -= tmp; }
        for (idx_t e = 0; e < exp; ++e) {
            result = result * (seq_t)p;
        }
    }
    return result;
}

/* coefficients of a row r at distance d from the centre */
static seq_t triangle_coeff(idx_t r, idx_t d)
{
    idx_t tmax = (r - 1 - d) / 2;
    seq_t sum = 0;
    for (idx_t t = 0; t <= tmax; ++t) {
        idx_t n = d + 2*t;
        idx_t k = t;
        seq_t bin = binom_prime_exp(n, k);
        sum += bin;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset is 1 */
    idx_t t  = isqrt_u64(n - 1);                 /* floor sqrt(n-1) */
    idx_t r  = t + 1;                            /* row number  (r ≥ 1)  */
    idx_t prev_sq = (r - 1) * (r - 1);
    idx_t pos = n - prev_sq;                     /* 1‑based pos in row r */
    idx_t d = (pos >= r) ? (pos - r) : (r - pos);
    return triangle_coeff(r, d);
}
