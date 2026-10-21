/*
 * A081227  a(n) is the number of digits in common between n and the n-th
 *          prime in base 10.
 * https://oeis.org/A081227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of distinct common decimal digits between n and
 *            the nth prime
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Build a table of the first n primes (1‑based).  Returns the array
   and the number of primes stored via out‑arguments.  The caller
   owns the returned array and must free it. */
static void build_primes(idx_t n, uint32_t **primes_out, size_t *count_out)
{
    size_t limit;
    if (n < 6) {
        limit = 15;               /* first 6 primes are < 15 */
    } else {
        double dn = (double)n;
        /* Rosser's bound: p_n < n(log n + log log n) for n >= 6 */
        limit = (size_t)(dn * (log(dn) + log(log(dn))) + 10.0);
    }

    /* Sieve of Eratosthenes up to 'limit' */
    char *is_prime = (char *)calloc(limit + 1, sizeof(char));
    if (!is_prime) {
        *primes_out = NULL; *count_out = 0; return;
    }
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    size_t sq = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= sq; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }

    /* Count primes to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
    if (cnt < n) { free(is_prime); *primes_out = NULL; *count_out = 0; return; }

    uint32_t *tbl = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!tbl) { free(is_prime); *primes_out = NULL; *count_out = 0; return; }

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) tbl[pos++] = (uint32_t)i;

    free(is_prime);
    *primes_out = tbl;
    *count_out = cnt;
}

seq_t a(idx_t n)
{
    /* Static cache of prime table */
    static uint32_t *primes = NULL;
    static size_t prime_count = 0;

    /* Rebuild table if we don't have enough primes yet */
    if (prime_count < (size_t)n) {
        uint32_t *new_tbl = NULL; size_t new_cnt = 0;
        build_primes(n, &new_tbl, &new_cnt);
        if (new_tbl) {
            free(primes);
            primes = new_tbl;
            prime_count = new_cnt;
        } else {
            return (seq_t)0;          /* out of memory or error */
        }
    }

    /* nth prime (1‑based) */
    uint32_t prime = primes[n - 1];

    /* Build a 10‑bit mask for the digits of n and the prime */
    unsigned mask_n = 0, mask_p = 0;
    idx_t tn = n;
    if (tn == 0) mask_n |= 1u << 0;          /* should never happen (n ≥ 1) */
    while (tn > 0) {
        mask_n |= 1u << (tn % 10);
        tn /= 10;
    }
    uint32_t tp = prime;
    if (tp == 0) mask_p |= 1u << 0;          /* never happens either */
    while (tp > 0) {
        mask_p |= 1u << (tp % 10);
        tp /= 10;
    }

    /* Count common bits = common distinct digits */
    unsigned common = mask_n & mask_p;
    unsigned cnt = 0;
    while (common) {
        common &= common - 1;
        ++cnt;
    }
    return (seq_t)cnt;
}
