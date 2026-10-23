/*
 * A100616  Let B(n)(x) be the Bernoulli polynomials as defined in A001898,
 *          with B(n)(1) equal to the usual Bernoulli numbers A027641/A027642.
 *          Sequence gives denominators of B(n)(2).
 * https://oeis.org/A100616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{j=0..n}
 *            binomial(n,j)*Bernoulli(n-j)*Bernoulli(j)
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 179, u64 n <= 719,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd_seq.h"

/* Greatest common divisor for seq_t, works for built‑in and arbitrary‑precision types. */

/* Precomputation of denominators of B_{2m}.  For 0 ≤ m ≤ 10000 this array
   holds D[2m] = Π{p prime | (p‑1) | 2m} p.  The denominator of an odd
   n > 1 is D[n‑1]; the denominator of an even n ≥ 4 is D[n], then both
   are divided by a gcd with n or n‑1 respectively. */
#define MAXN 20000          /* enough for n up to 10000 */
static seq_t denom_arr[MAXN + 1];
static bool initialized = false;

static void compute_denoms(void) {
    static bool sieve[MAXN + 2];
    static bool sieve_done = false;
    if (!sieve_done) {
        for (int i = 0; i <= MAXN + 1; ++i) sieve[i] = true;
        sieve[0] = sieve[1] = false;
        int lim = MAXN + 1;
        for (int i = 2; i * i <= lim; ++i) {
            if (sieve[i]) {
                for (int j = i * i; j <= lim; j += i) sieve[j] = false;
            }
        }
        sieve_done = true;
    }

    /* initialise denominators to 1 */
    for (int i = 0; i <= MAXN; ++i) denom_arr[i] = (seq_t)1;

    /* for each prime p, multiply its value into all multiples of p‑1 */
    for (int p = 2; p <= MAXN + 1; ++p) {
        if (!sieve[p]) continue;
        int q = p - 1;          /* q > 0 for all primes p≥2 */
        for (int e = q; e <= MAXN; e += q) {
            denom_arr[e] *= (seq_t)p;
        }
    }

    initialized = true;
}

/* Main function returning the sequence value at index n. */
seq_t a(idx_t n) {
    if (!initialized) compute_denoms();

    if (n == 0) return (seq_t)1;          /* B0 = 1 */
    if (n == 1) return (seq_t)1;          /* S1 = –1 → |den| = 1 */
    if (n == 2) return (seq_t)6;          /* special case: 5/6 → denom 6 */

    if ((n & 1) == 1) {                  /* odd n > 1 */
        seq_t d = denom_arr[(int)(n - 1)];          /* denom of B_{n-1} */
        seq_t g = gcd_seq(d, (seq_t)n);           /* simplify by factor n */
        return d / g;
    } else {                              /* even n ≥ 4 */
        seq_t d = denom_arr[(int)n];              /* denom of B_n */
        seq_t g = gcd_seq(d, (seq_t)(n - 1));      /* simplify by factor n‑1 */
        return d / g;
    }
}
