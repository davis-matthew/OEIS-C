/*
 * A212429  a(n) is the LCM of denominators of polynomials of degree n which
 *          are integer-valued on primes together with their first divided
 *          differences.
 * https://oeis.org/A212429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prod_{p prime} p^{w_p(n-1)}, where w_p(m) = Sum_{k>=0}
 *            floor(m / ((p-1)*p^k)) - t_{p,m} and t_{p,m} = floor(log_p(
 *            m/(p-1) )) + 1 (t=0 if m < p-1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

/* Generate all primes up to "limit" (inclusive) using a simple sieve. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= primes_limit) return;

    if (primes) { free(primes); primes = NULL; }
    if (primes_cnt) { free(primes); primes_cnt = 0; }

    /* allocate array for composite flags; 0 = prime candidate */
    size_t size = (size_t)(limit + 1);
    bool *is_comp = (bool*)calloc(size, sizeof(bool));
    if (!is_comp) return;    /* allocation failed – omit prime computing */

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!primes) { free(is_comp); return; }

    /* Sieve */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if ((uint64_t)i * (uint64_t)i <= (uint64_t)limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }

    /* Store primes */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) primes[idx++] = (uint32_t)i;

    primes_cnt = cnt;
    primes_limit = limit;
    free(is_comp);
}

/* Fast exponentiation for seq_t (supports unsigned int, 64‑bit, or big ints). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* not part of sequence but safe */

    idx_t m = n - 1;                /* use m = n-1 in formula */
    if (m == 0) return 1;           /* a(1) = 1 */

    ensure_primes_up_to(m);         /* primes needed are <= m */

    seq_t result = 1;

    for (size_t p_idx = 0; p_idx < primes_cnt; ++p_idx) {
        idx_t p = (idx_t)primes[p_idx];
        if (p > m) break;           /* subsequent primes > m have exponent 0 */

        /* Compute t_{p,m} */
        idx_t t = 0;
        idx_t limit = m / (p - 1);   /* floor division; 0 if m < p-1 */
        idx_t power = 1;
        while (power <= limit) {
            ++t;
            power *= p;
        }

        /* Sum floor(m / ((p-1)*p^k)) */
        idx_t sum = 0;
        power = 1;
        idx_t denom;
        while (true) {
            denom = (p - 1) * power;
            if (denom > m) break;
            sum += m / denom;
            power *= p;
        }

        idx_t exp = sum - t;        /* w_p(m) */
        if (exp > 0)
            result *= pow_seq((seq_t)p, exp);
    }

    return result;
}
