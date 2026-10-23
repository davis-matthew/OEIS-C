/*
 * A102066  Sum of the first n primes, mod 6.
 * https://oeis.org/A102066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (Σ_{k=1..n} p_k) mod 6     (p_k is the k‑th prime, p_1 = 2)
 * The function keeps a cache of primes and their cumulative sums modulo 6.
 */
#include <stdlib.h>
#include <stdint.h>

/* Cache of primes and cumulative sums modulo 6. */
static uint32_t *prime_list = NULL;    /* primes, 0‑based */
static seq_t   *cum_sum    = NULL;    /* cumulative sum mod 6 */
static uint32_t prime_cnt   = 0;       /* number of primes currently stored */

/* Grow the cache so that it contains at least `need` primes. */
static void
ensure_primes(uint32_t need)
{
    if (prime_cnt >= need)
        return;

    /* Allocate more space. */
    size_t new_cap = prime_cnt ? prime_cnt * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    prime_list = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    cum_sum    = (seq_t   *)realloc(cum_sum,    new_cap * sizeof(seq_t));

    /* Generate missing primes. */
    for (uint32_t i = prime_cnt; i < need; ++i) {
        uint32_t cand = (i == 0) ? 2 : prime_list[i - 1] + 1;
        for (;;) {
            int is_prime = 1;
            for (uint32_t j = 0; j < prime_cnt && ((uint64_t)prime_list[j] * prime_list[j] <= cand); ++j) {
                if (cand % prime_list[j] == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                prime_list[prime_cnt] = cand;
                if (prime_cnt == 0)
                    cum_sum[prime_cnt] = cand % 6;                     /* first term */
                else
                    cum_sum[prime_cnt] = ((seq_t)(cand % 6) + cum_sum[prime_cnt - 1]) % 6;
                ++prime_cnt;
                break;
            }
            ++cand;
        }
    }
}

/* Public function requested by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1 in OEIS notation */
    ensure_primes((uint32_t)n);
    return cum_sum[n - 1];
}
