/*
 * A097859  a(m,0) = a(m,m) = 2; a(m,n) = sum of primes (with repetition)
 *          dividing a(m-1,n-1) and of primes (with repetition) dividing
 *          a(m-1,n).
 * https://oeis.org/A097859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(m,0)=a(m,m)=2;
 *           a(m,n)=sum of prime factors (with multiplicity)
 *                of a(m-1,n-1) plus that of a(m-1,n).
 *           a(n) enumerates the triangle row‑major with
 *           index  n = m*(m+1)/2 + k  (0 ≤ k ≤ m).
 *
 * Implements a recursive definition by dynamic programming,
 * computing rows only up to the needed maximum.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Helpers for prime generation */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static uint64_t  prime_limit = 0;   /* last integer checked */

/* Ensure primes up to a given limit (inclusive) */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;   /* already initialized */

    const uint64_t LIMIT = 1000000;   /* covers all needed primes */
    uint8_t *is_prime = (uint8_t *)malloc(LIMIT + 1);
    memset(is_prime, 1, LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = i;

    free(is_prime);
    prime_limit = LIMIT;
}

/* Sum of prime factors of v, counted with multiplicity */
static seq_t sum_prime_factors(seq_t v)
{
    init_primes();   /* guarantee primes are ready */

    seq_t sum = 0;
    seq_t temp = v;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        seq_t prime_seq = (seq_t)p;
        if (prime_seq * prime_seq > temp)
            break;
        while (temp % prime_seq == (seq_t)0) {
            sum += prime_seq;
            temp /= prime_seq;
        }
    }

    if (temp > (seq_t)1)
        sum += temp;   /* remaining prime factor > sqrt(v) */

    return sum;
}

/* Triangle storage */
static seq_t *triangle   = NULL;
static idx_t   max_m     = (idx_t)(-1);   /* no rows computed yet */

/* Ensure rows 0..target_m are computed */
static void ensure_triangle_up_to(idx_t target_m)
{
    if (triangle == NULL) {
        /* initial allocation for row 0 */
        triangle = (seq_t *)malloc(sizeof(seq_t));
        triangle[0] = (seq_t)2;
        max_m = 0;
    }
    if (target_m <= max_m)
        return; /* already done */

    /* Resize array for new rows */
    unsigned __int128 new_size128 = (unsigned __int128)(target_m + 1) * (target_m + 2) / 2;
    size_t new_size = (size_t)new_size128;

    triangle = (seq_t *)realloc(triangle, new_size * sizeof(seq_t));

    /* Compute new rows */
    for (idx_t m = max_m + 1; m <= target_m; ++m) {
        idx_t prev_start = (unsigned __int128)(m - 1) * m / 2;
        idx_t start      = (unsigned __int128)m * (m + 1) / 2;

        for (idx_t k = 0; k <= m; ++k) {
            seq_t val;
            if (k == 0 || k == m)
                val = (seq_t)2;
            else {
                seq_t left  = triangle[prev_start + k - 1];
                seq_t right = triangle[prev_start + k];
                val = sum_prime_factors(left) + sum_prime_factors(right);
            }
            triangle[start + k] = val;
        }
    }

    max_m = target_m;
}

/* Public entry point */
seq_t a(idx_t n)
{
    /* Find row m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2 */
    idx_t m = 0;
    while (1) {
        unsigned __int128 upper = (unsigned __int128)(m + 1) * (m + 2) / 2;
        if ((unsigned __int128)n < upper)
            break;
        ++m;
    }

    idx_t k = n - (unsigned __int128)m * (m + 1) / 2;
    ensure_triangle_up_to(m);
    return triangle[(unsigned __int128)m * (m + 1) / 2 + k];
}
