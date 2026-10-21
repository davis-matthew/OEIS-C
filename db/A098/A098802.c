/*
 * A098802  Greatest prime factors in Pascal's triangle (read by rows).
 * https://oeis.org/A098802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   greatest prime factor of the binomial coefficient read row‑wise
 *            from Pascal's triangle; a(0)=1, a(1)=1, ... a(n) is the greatest
 *            prime factor of C(r,k), where r is the row containing the n‑th
 *            element and k the column within that row. For entries equal to 1
 *            we return 1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* helper: number of times prime p divides m! */
static inline idx_t vp_factorial(idx_t m, uint32_t p) {
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/* dynamic list of primes up to a given limit */
static uint32_t *g_primes = NULL;
static uint32_t g_prime_cnt = 0;
static uint32_t g_prime_limit = 0;

static void ensure_primes_up_to(uint32_t limit) {
    if (limit <= g_prime_limit) return;

    size_t size = (size_t)limit + 1;
    bool *is_comp = (bool*)calloc(size, sizeof(bool));
    if (!is_comp) exit(1);

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_comp[j] = true;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (!is_comp[i]) ++cnt;

    uint32_t *new_primes = (uint32_t*)realloc(g_primes, cnt * sizeof(uint32_t));
    if (!new_primes) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (!is_comp[i]) new_primes[idx++] = (uint32_t)i;

    g_primes = new_primes;
    g_prime_cnt = (uint32_t)cnt;
    g_prime_limit = limit;

    free(is_comp);
}

seq_t a(idx_t n) {
    /* Find row r such that n lies in row r */
    long double val = sqrtl(8.0L * (long double)n + 1.0L);
    uint64_t r = (uint64_t)((val - 1.0L) / 2.0L);

    /* Correct possible rounding error */
    while ( (__int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ( (__int128)r * (r + 1) / 2 > n) --r;

    /* offset of first element of row r */
    __int128 offset = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)offset;

    if (k == 0 || k == r) return (seq_t)1;   /* C(r,0)=C(r,r)=1 */

    ensure_primes_up_to((uint32_t)r);

    /* largest prime factor of C(r,k) by Legendre's formula */
    for (int i = (int)g_prime_cnt - 1; i >= 0; --i) {
        uint32_t p = g_primes[i];
        if (p > r) continue;
        idx_t e = vp_factorial(r, p) - vp_factorial(k, p) - vp_factorial(r - k, p);
        if (e > 0) return (seq_t)p;
    }
    return (seq_t)1;   /* fallback, should not happen */
}
