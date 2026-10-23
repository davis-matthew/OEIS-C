/*
 * A127481  Triangle T(n,k) read by rows: T(n,k) = sum_{l=k..n, l|n, k|l}
 *          l*phi(k).
 * https://oeis.org/A127481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row(n), col(n)) where row(n) is the smallest r with
 *            r(r+1)/2 ≥ n, col(n) = n – (r‑1)r/2. T(n,k) =
 *            k*phi(k)*sigma(n/k) if n % k == 0, 0 otherwise. (phi = Euler's
 *            totient, sigma = sum of divisors.)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes, col_of_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/col_of_index.h"

/* ---------- Helper to obtain row and column ---------- */
static idx_t row_of_index(idx_t idx)
{
    if (idx <= 1) return 1;
    idx_t lo = 0, hi = 1;
    /* grow hi until it covers idx */
    while ( (__int128)hi * (hi + 1) / 2 < idx ) {
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= idx)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;   /* smallest r with r(r+1)/2 >= idx */
}

/* ---------- Prime table (built once) ---------- */
static bool primes_init = false;
static idx_t *prime_list = NULL;
static size_t prime_cnt  = 0;

static void init_prime_list(void)
{
    if (primes_init) return;
    /* 100000 is ample for all n reachable in tests (row ≤ ~10⁴). */
    build_primes(100000, &prime_list, &prime_cnt);
    primes_init = true;
}

/* ---------- Arithmetic helpers ---------- */
static seq_t compute_phi(uint64_t n)
{
    seq_t phi = 1;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) { m /= p; ++exp; }
            /* term = p^(exp-1) * (p-1) */
            seq_t p_pow = 1;
            for (int j = 0; j < exp - 1; ++j) p_pow *= (seq_t)p;
            phi *= p_pow * ((seq_t)p - 1);
        }
    }
    if (m > 1) {
        seq_t p = (seq_t)m;
        phi *= (p - 1);
    }
    return phi;
}

static seq_t compute_sigma(uint64_t n)
{
    seq_t sigma = 1;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) { m /= p; ++exp; }
            /* factor = (p^(exp+1)-1)/(p-1) */
            seq_t p_pow = 1;
            for (int j = 0; j < exp + 1; ++j) p_pow *= (seq_t)p;
            seq_t numerator = p_pow - 1;
            seq_t denom = (seq_t)p - 1;
            seq_t factor = numerator / denom;
            sigma *= factor;
        }
    }
    if (m > 1) {
        /* remaining prime factor, exp = 1 */
        seq_t p = (seq_t)m;
        sigma *= (p + 1);   /* (p^2-1)/(p-1) = p+1 */
    }
    return sigma;
}

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, but guard against 0 */
    init_prime_list();

    idx_t row = row_of_index(n);
    idx_t col = col_of_index(n, row);

    if (row % col != 0) return 0;

    uint64_t m = row / col;        /* m fits in uint64_t */
    seq_t phi_col = compute_phi((uint64_t)col);
    seq_t sigma_m   = compute_sigma(m);

    seq_t result = (seq_t)col * phi_col * sigma_m;
    return result;
}
