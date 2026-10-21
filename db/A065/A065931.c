/*
 * A065931  Triangle of coefficients of Bessel polynomials {y_n(x)}'.
 * https://oeis.org/A065931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattening of the triangle of coefficients of Bessel
 *            polynomials. For row r (1‑based) and column k (0‑based) T(r,k) =
 *            ((r+k+1)!/(k!*(r-k-1)!))*(1/2)^(k+1). The index n is mapped to
 *            row r and column k by r = smallest integer with r(r+1)/2 ≥ n k =
 *            n - r(r-1)/2 - 1. The function returns...
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 53, u64 n <= 150, u128 n <= 430,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* A small, static list of all primes ≤ 281 (the maximum needed for n≤10000). */
static const int primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,
    67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,
    139,149,151,157,163,167,173,179,181,191,193,197,199,
    211,223,227,229,233,239,241,251,257,263,269,271,277,281
};
static const int prime_cnt = sizeof(primes)/sizeof(primes[0]);

/* Exponent of prime p in N! (Legendre's formula).  N ≤ 282. */
static unsigned int exp_in_fact(idx_t N, unsigned int p)
{
    unsigned int exp = 0;
    idx_t d = p;
    while (d <= N) {
        exp += N / d;
        d *= p;
    }
    return exp;
}

/* Fast exponentiation for seq_t (works for built‑in and arbitrary‑precision).
   No division is used. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

/* Main sequence function.  Returns a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    /* Find row r and column k from the flat index n. */
    idx_t r = 1;
    idx_t cumulative = 0;           /* number of terms before current row */
    while (cumulative + r < n) {
        cumulative += r;
        ++r;
    }
    idx_t k = n - cumulative - 1;   /* 0‑based column */

    if (k == 0) {                   /* T(r,0) = r*(r+1)/2 */
        return ((seq_t)r) * ((seq_t)r + 1) / 2;
    }

    /* Compute the multinomial part using prime exponents. */
    idx_t N1 = r + k + 1;   /* (r+k+1)!   */
    idx_t N2 = r - k - 1;   /* (r-k-1)!   */
    idx_t N3 = k;           /* k!         */

    seq_t result = 1;
    for (int pi = 0; pi < prime_cnt; ++pi) {
        unsigned int p = primes[pi];
        if (p > N1) break;
        unsigned int e = exp_in_fact(N1, p);
        e -= exp_in_fact(N2, p);
        e -= exp_in_fact(N3, p);
        if (p == 2)
            e -= (k + 1);      /* division by 2^(k+1) */
        if (e > 0)
            result *= pow_seq((seq_t)p, e);
    }
    return result;
}
