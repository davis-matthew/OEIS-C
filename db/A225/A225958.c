/*
 * A225958  O.g.f.: exp( Sum_{n>=1} (sigma(2*n^3) - sigma(n^3)) * x^n/n ).
 * https://oeis.org/A225958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in the expansion of exp( Sum_{k>=1}
 *            (sigma(2*k^3)-sigma(k^3)) * x^k / k ) where sigma is the sum of
 *            divisors
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Auxiliary small‑power routine (works for any seq_t). */
static inline seq_t
pow_seq(seq_t base, unsigned int exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* Small‑prime‑factor table (lazy, up to current limit). */
static idx_t *spf = NULL;
static idx_t spf_cap = 0;

static void
ensure_spf(idx_t n)
{
    if (n < spf_cap) return;
    if (spf) free(spf);
    spf = (idx_t*)calloc((size_t)(n + 1), sizeof(idx_t));
    if (!spf) { exit(1); }
    spf[0] = 0;
    if (n >= 1) spf[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_cap = n + 1;
}

/* Compute f(k) = sigma(2*k^3) – sigma(k^3). */
static seq_t
sigma_diff(idx_t k)
{
    seq_t sigma_k3   = (seq_t)1;
    seq_t sigma_2k3  = (seq_t)1;
    idx_t temp = k;
    while (temp > 1) {
        idx_t p   = spf[temp];
        unsigned int cnt = 0;
        while (temp % p == 0) { temp /= p; ++cnt; }

        unsigned int e_k3  = 3 * cnt;                         /* exponent in k^3  */
        unsigned int e_2k3 = (p == 2U) ? (e_k3 + 1) : e_k3;   /* exponent in 2*k^3 */

        seq_t term_k3  = (pow_seq((seq_t)p, e_k3 + 1) - (seq_t)1) / ((seq_t)p - (seq_t)1);
        seq_t term_2k3 = (pow_seq((seq_t)p, e_2k3 + 1) - (seq_t)1) / ((seq_t)p - (seq_t)1);

        sigma_k3  *= term_k3;
        sigma_2k3 *= term_2k3;
    }

    /* Extra factor for the 2 in 2*k^3 when k is odd. */
    if ((k & 1ULL) != 0ULL) {
        /* sigma(2^1) = 1 + 2 = 3 */
        sigma_2k3 *= (seq_t)3;
    }

    return sigma_2k3 - sigma_k3;
}

/* Memoised storage for f(k) and a(k). */
static seq_t *f_vals = NULL;   /* f_vals[n] for n >= 1 */
static seq_t *a_vals = NULL;   /* a_vals[n] for n >= 0 */
static idx_t  max_computed = 0;   /* highest n with a_vals[n] computed */
static idx_t  f_cap  = 0;         /* current capacity of f_vals   */
static idx_t  a_cap  = 0;         /* current capacity of a_vals   */

/* Ensure that a_vals[0..n] and f_vals[1..n] are defined. */
static void
ensure_computed(idx_t n)
{
    if (n <= max_computed) return;

    /* Build spf up to n. */
    ensure_spf(n);

    /* Resize or initialise f_vals. */
    if (f_vals == NULL) {
        f_vals = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!f_vals) exit(1);
        f_cap = n + 1;
    } else if (f_cap <= n) {
        seq_t *tmp = (seq_t*)realloc(f_vals, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        f_vals = tmp;
        f_cap  = n + 1;
    }

    /* Resize or initialise a_vals. */
    if (a_vals == NULL) {
        a_vals = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!a_vals) exit(1);
        a_cap = n + 1;
    } else if (a_cap <= n) {
        seq_t *tmp = (seq_t*)realloc(a_vals, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        a_vals = tmp;
        a_cap  = n + 1;
    }

    /* Initialise a_vals[0] if this is the first pass. */
    if (max_computed == 0) a_vals[0] = (seq_t)1;

    /* Compute values from max_computed+1 up to n. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        f_vals[k] = sigma_diff(k);

        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= k; ++j) {
            sum += f_vals[j] * a_vals[k - j];
        }
        a_vals[k] = sum / (seq_t)k;
    }

    max_computed = n;
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    ensure_computed(n);
    return a_vals[n];
}
