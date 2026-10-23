/*
 * A104244  Suppose m = Product_{i=1..k} p_i^e_i, where p_i is the i-th prime
 *          number and each e_i is a nonnegative integer. Then we can define
 *          P_m(x) = Sum_{i=1..k} e_i*x^(i-1). The sequence is the square
 *          array A(n,m) = P_m(n) read by descending antidiagonals.
 * https://oeis.org/A104244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = P_m(n) where (row=r, column=m) are obtained by reading
 *            the square array A by descending antidiagonals. Here P_m(x) =
 *            Σ_i e_i * x^(i-1) and the prime factorisation of m is m = Π_{i}
 *            p_i^e_i
 * Range:     u8 n <= 84, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */

/* List of primes up to 139 – sufficient for m ≤ 141 (max diagonal for n≈10000). */
static const int primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
    59,61,67,71,73,79,83,89,97,101,103,107,109,113,
    127,131,137,139
};
static const size_t prime_count = sizeof(primes) / sizeof(primes[0]);

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for non‑positive indices */

    /* Find the diagonal k such that T(k-1) < n ≤ T(k), where T(k)=k(k+1)/2. */
    idx_t k = 1;
    for (;;) {
        idx_t tri = k * (k + 1) / 2;
        if (tri >= n) break;
        ++k;
    }

    idx_t prev = (k - 1) * k / 2;          /* number of elements before diagonal k */
    idx_t offset = n - prev - 1;           /* 0‑based position within diagonal k */
    idx_t r = offset + 1;                  /* row number */
    idx_t m = k - offset;                  /* column number (m = k - r + 1) */

    /* Evaluate P_m(r).  r^(i-1) is accumulated in pow_r. */
    seq_t result = 0;
    seq_t pow_r = 1;            /* r^0 at start */
    idx_t tmp = m;              /* factorisation workbench */

    for (size_t i = 0; i < prime_count; ++i) {
        int p = primes[i];
        int exp = 0;
        while (tmp % p == 0) {
            ++exp;
            tmp /= p;
        }
        if (exp) {
            result += (seq_t)exp * pow_r;
        }
        pow_r *= (seq_t)r;      /* advance to next power r^(i+1) */
    }

    /* If any prime factor > 139 remains – it is a single prime >139
       which can only happen for m = 140 (2^2·5·7) or m = 141 (3·47),
       already handled.  For safety handle any remainder. */
    if (tmp > 1) {
        result += pow_r;        /* exponent is 1, power r^(prime_count) */
    }

    return result;
}
