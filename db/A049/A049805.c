/*
 * A049805  Triangular array T read by rows: T(n,k) is the number of Farey
 *          fractions of order n that are <= 1/k for k=1..n, for n>=1.
 * https://oeis.org/A049805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 406, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049805:  T(n,k) – count of Farey fractions of order n that are ≤ 1/k.
 *
 * The flat index n is converted to a row r and column k by inverting the
 * triangular numbers:  n = r*(r‑1)/2 + k   (1 ≤ k ≤ r).
 *
 * For a given r and k the value is
 *
 *     T(r,k) = 1 + Σ_{d=1}^{r} μ(d) * G( ⌊r/d⌋ )
 *
 * where μ is the Möbius function and
 *
 *   G(t) = Σ_{m=1}^{t} ⌊m / k⌋
 *        = k*(q-1)*q/2  +  q*(t - q*k + 1)            with  q = t / k .
 *
 * This follows from counting reduced fractions a/b ≤ 1/k with 1 ≤ b ≤ r.
 *
 * The implementation uses a linear sieve to compute μ up to the
 * required row r and a grouping trick over d to achieve O(√r) time per call.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- Möbius cache ---------- */
static int   *mu_table  = NULL;  /* μ[1..size] */
static int   *mu_pref   = NULL;  /* prefix sums of μ */
static idx_t  mu_size   = 0;

/* ensure μ and its prefix sums are computed up to LIMIT (inclusive) */
static void
ensure_mu(idx_t LIMIT)
{
    if (LIMIT <= mu_size) return;

    /* allocate new tables */
    int *mu  = (int*)calloc((size_t)(LIMIT + 1), sizeof(int));
    int *pref = (int*)calloc((size_t)(LIMIT + 1), sizeof(int));
    if (!mu || !pref) abort();

    /* linear sieve for μ */
    int *is_comp = (int*)calloc((size_t)(LIMIT + 1), sizeof(int));
    int *primes  = (int*)malloc((size_t)(LIMIT + 1) * sizeof(int));
    size_t pcnt = 0;

    mu[1] = 1;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[(size_t)i]) {
            mu[(size_t)i] = -1;
            primes[pcnt++] = (int)i;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            idx_t p = primes[j];
            idx_t prod = i * p;
            if (prod > LIMIT) break;
            is_comp[(size_t)prod] = 1;
            if (i % p == 0) {
                mu[(size_t)prod] = 0;
                break;
            } else {
                mu[(size_t)prod] = -mu[(size_t)i];
            }
        }
    }

    free(is_comp);
    free(primes);

    /* prefix sums */
    pref[0] = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        pref[(size_t)i] = pref[(size_t)i - 1] + mu[(size_t)i];
    }

    /* replace old tables */
    free(mu_table);
    free(mu_pref);
    mu_table = mu;
    mu_pref  = pref;
    mu_size  = LIMIT;
}

/* compute G(t,k) = Σ_{m=1}^{t} ⌊m/k⌋ efficiently */
static inline seq_t
compute_G(idx_t t, idx_t k)
{
    if (k == 0) return 0;                 /* safety, never occurs */
    idx_t q = t / k;                      /* floor(t/k) */
    if (q == 0) return 0;                 /* all terms are zero */

    /* part1 = k*(q-1)*q/2  */
    seq_t part1 = (seq_t)k * (seq_t)(q - 1) * (seq_t)q / 2;
    /* part2 = q*(t - q*k + 1) */
    seq_t part2 = (seq_t)q * (seq_t)(t - (idx_t)q * k + 1);

    return part1 + part2;
}

/* inverse triangular numbering: find r such that N = r*(r-1)/2 + k */
static inline idx_t
row_from_index(idx_t N)
{
    if (N == 0) return 0;
    long double approx = sqrtl((long double)(8 * (long double)N + 1));
    idx_t r = (idx_t)((approx - 1.0L) / 2.0L);

    /* correct possible rounding errors */
    while (r * (r + 1) / 2 < N) ++r;
    while (r > 1 && (r - 1) * r / 2 >= N) --r;
    return r;
}

/* main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but return 0 */

    /* decode row and column */
    idx_t r = row_from_index(n);
    idx_t total_before = (r - 1) * r / 2;
    idx_t k = n - total_before;           /* 1 ≤ k ≤ r */

    /* precompute Möbius up to r */
    ensure_mu(r);

    seq_t result = 1;                    /* count of 0/1 */

    idx_t l = 1;
    while (l <= r) {
        idx_t t   = r / l;                 /* value of ⌊r/l⌋ */
        idx_t rd  = r / t;                 /* last d with the same t */

        /* sum of μ from l to rd */
        int sum_mu_int = mu_pref[(size_t)rd] - mu_pref[(size_t)l - 1];
        seq_t sum_mu   = (seq_t)sum_mu_int;

        seq_t g_val = compute_G(t, k);
        result += sum_mu * g_val;

        l = rd + 1;
    }

    return result;
}
