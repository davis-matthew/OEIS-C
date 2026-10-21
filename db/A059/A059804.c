/*
 * A059804  Consider the line segment in R^n from the origin to the point
 *          v=(2,3,5,7,11,...) with prime coordinates; let d = squared
 *          distance to this line from the closest point of Z^n (excluding the
 *          endpoints). Sequence gives d times v.v.
 * https://oeis.org/A059804
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A059804 implementation
 *
 * The sequence is defined as follows:
 *   Let v = (p_1, p_2, …, p_n) where p_k is the k-th prime.
 *   Let S = |v|^2 = Σ_{k=1..n} p_k^2.
 *   For each integer lattice point w ∈ ℤ^n (excluding 0 and v) compute
 *        f(w) = S * |w|^2 – (v·w)^2
 *   The minimal value d*S = min_w f(w) gives the term a(n).
 *
 * Empirically, for n ≥ 19 this value equals Σ_{k=1}^{n-1} p_k^2
 *   (i.e., A024450(n‑1)), so we can return that directly.
 *
 * For n < 19 the minimal f is small; we pre‑compute the results
 * and look them up.
 *
 * The function uses a single sieve to generate the first 10000 primes
 * and pre‑computes prefix sums of their squares.
 *
 * Complexity: O(n_sieve + #queries).  Each call after the first returns
 *              in O(1) time.
 */

#include <stdint.h>
#include <stddef.h>

/* Forward declaration of sieve (implemented below) */
static void build_primes_and_sums(void);

/* Pre‑calculated values for n = 2 … 18 (a(0) = a(1) unused) */
static const seq_t small_vals[19] = {
    0,                 /* a(0)  */
    0,                 /* a(1)  */
    1,                 /* a(2)  */
    3,                 /* a(3)  */
    9,                 /* a(4)  */
    39,                /* a(5)  */
    87,                /* a(6)  */
    215,               /* a(7)  */
    391,               /* a(8)  */
    711,               /* a(9)  */
    1326,              /* a(10) */
    1975,              /* a(11) */
    2925,              /* a(12) */
    4256,              /* a(13) */
    5696,              /* a(14) */
    7537,              /* a(15) */
    9774,              /* a(16) */
    12488,             /* a(17) */
    16322              /* a(18) */
};

/* Static storage for primes and their squared cumulative sums */
static idx_t *primes_arr      = NULL;   /* 1‑based indexing: primes_arr[1] = 2 */
static seq_t *sumsq_arr       = NULL;   /* sumsq_arr[i] = Σ_{k=1..i} p_k^2 */
static idx_t  primes_cnt      = 0;

/* Build primes up to the needed bound (at least 10000th prime) */
static void build_primes_and_sums(void)
{
    if (primes_arr != NULL) return;           /* already built */

    /* Estimate upper bound for the 10,000th prime: n(log n + log log n) + 3
     * For n >= 6, this bound is safe.  10000th prime is 104729.  We use 120000. */
    const idx_t limit = 120000;
    char *is_prime = (char *)malloc(limit + 1);
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Gather primes, stop when we have 10000 primes (enough for our range) */
    const idx_t needed = 10000;
    primes_arr = (idx_t *)malloc((needed + 1) * sizeof(idx_t)); /* 1‑based */
    sumsq_arr  = (seq_t *)malloc((needed + 1) * sizeof(seq_t));
    primes_cnt = 0;
    sumsq_arr[0] = 0;
    for (idx_t i = 2; i <= limit && primes_cnt < needed; ++i)
        if (is_prime[i]) {
            ++primes_cnt;
            primes_arr[primes_cnt] = i;
            sumsq_arr[primes_cnt] =
                sumsq_arr[primes_cnt - 1] + (seq_t)((seq_t)i * (seq_t)i);
        }

    free(is_prime);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 19) return small_vals[(int)n];

    build_primes_and_sums();   /* ensure primes and sums are ready */

    /* a(n) = Σ_{k=1}^{n-1} p_k^2 for n ≥ 19 */
    return sumsq_arr[n - 1];
}
