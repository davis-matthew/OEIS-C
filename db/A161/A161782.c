/*
 * A161782  a(n) = sum of all numbers from and including (prime(n+1)-prime(n))
 *          to and including (prime(n+2)-prime(n).)
 * https://oeis.org/A161782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{x=prime(n+1)-prime(n)}^{prime(n+2)-prime(n)} x
 * Range:     u8 n <= 152, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = ((prime(n+1)+prime(n+2)-2*prime(n))*(prime(n+2)-prime(n+1)+1))/2 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system. */

static uint64_t *primes = NULL;   /* array of primes, 0‑based */
static size_t primes_cnt = 0;     /* number of primes stored */
static size_t primes_limit = 0;   /* numeric upper bound used */

/* Very small sieve – sufficient for n up to about 10 000. */
static void generate_primes_up_to(size_t limit, uint64_t **out, size_t *cnt)
{
    if (limit < 2) {
        *out = NULL;
        *cnt = 0;
        return;
    }
    bool *is_comp = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_comp) {
        *out = NULL;
        *cnt = 0;
        return;
    }
    for (size_t i = 0; i <= limit; ++i) is_comp[i] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++count;

    uint64_t *pr = (uint64_t*)malloc(count * sizeof(uint64_t));
    if (!pr) {
        free(is_comp);
        *out = NULL;
        *cnt = 0;
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) pr[idx++] = (uint64_t)i;

    free(is_comp);
    *out = pr;
    *cnt = count;
}

/* Ensure we have at least indices n,n+1,n+2 stored (1‑based). */
static void ensure_primes(idx_t n)
{
    size_t required = (size_t)(n) + 2;              /* need indices n, n+1, n+2 */
    if (primes_cnt >= required)
        return;

    size_t new_limit = primes_limit ? primes_limit * 2 : 100;
    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;

    while (1) {
        generate_primes_up_to(new_limit, &new_primes, &new_cnt);
        if (new_cnt >= required)
            break;
        free(new_primes);
        new_limit *= 2;
    }

    free(primes);
    primes = new_primes;
    primes_cnt = new_cnt;
    primes_limit = new_limit;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    ensure_primes(n);

    seq_t p_n   = (seq_t)primes[(size_t)(n) - 1];
    seq_t p_np1 = (seq_t)primes[(size_t)(n)    ];
    seq_t p_np2 = (seq_t)primes[(size_t)(n) + 1];

    seq_t term1 = (p_np1 + p_np2) - (seq_t)2 * p_n;
    seq_t term2 = (p_np2 - p_np1) + (seq_t)1;
    return (term1 * term2) / (seq_t)2;
}
