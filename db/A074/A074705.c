/*
 * A074705  a(1)=4; for n>1: number of primes between squares of (n-1)-th and
 *          (n+1)-th primes.
 * https://oeis.org/A074705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ primes p | p_{n-1}^2 < p < p_{n+1}^2 }, a(1) = 4
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   estimate_prime_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/estimate_prime_limit.h"

/* ------------------------------------------------------------------ */
/* Prime generation – up to a required index.                          */
/* ------------------------------------------------------------------ */
static int   *prime_list = NULL;    /* 0‑based array of primes       */
static size_t prime_cnt  = 0;       /* number of primes stored       */
static int    primes_inited = 0;    /* flag                            */

/* Sieve of Eratosthenes to generate all primes up to `limit`. */
static void generate_primes_up_to(idx_t n_needed)
{
    if (primes_inited && prime_cnt >= (size_t)n_needed) return;

    int limit = estimate_prime_limit(n_needed);
    /* Allocate temporary boolean array for the sieve */
    char *is_prime = (char*)malloc((size_t)limit + 1);
    if (!is_prime) return;
    size_t i;
    for (i = 0; i <= (size_t)limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (i = 2; i * i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes and allocate storage */
    size_t cnt = 0;
    for (i = 2; i <= (size_t)limit; ++i) if (is_prime[i]) ++cnt;
    int *pr = (int*)malloc(cnt * sizeof(int));
    if (!pr) {
        free(is_prime);
        return;
    }
    size_t idx = 0;
    for (i = 2; i <= (size_t)limit; ++i) if (is_prime[i]) pr[idx++] = (int)i;

    free(is_prime);
    free(prime_list);
    prime_list = pr;
    prime_cnt  = cnt;
    primes_inited = 1;
}

/* ------------------------------------------------------------------ */
/* Count primes in a closed interval [low, high] using a segmented sieve */
/* ------------------------------------------------------------------ */
static seq_t count_primes_interval(uint64_t low, uint64_t high)
{
    if (high < low) return 0;
    size_t seg_size = (size_t)(high - low + 1);
    char *seg = (char*)malloc(seg_size);
    if (!seg) return 0;
    memset(seg, 1, seg_size);

    /* sqrt(high) – limit for primes to use in the segment sieve */
    uint64_t sqrt_high = (uint64_t)floorl(sqrtl((long double)high));
    for (size_t k = 0; k < prime_cnt && (uint64_t)prime_list[k] <= sqrt_high; ++k) {
        uint64_t p = (uint64_t)prime_list[k];
        uint64_t start = (low + p - 1) / p * p;
        if (start < p * p) start = p * p;
        for (uint64_t j = start; j <= high; j += p)
            seg[(size_t)(j - low)] = 0;
    }
    /* Count marked primes (those still set to 1) */
    size_t count = 0;
    for (size_t i = 0; i < seg_size; ++i)
        if (seg[i]) ++count;

    free(seg);
    return (seq_t)count;
}

/* ------------------------------------------------------------------ */
/* Main OEIS function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below the offset */
    if (n == 1) return 4;          /* special initial value */

    /* Ensure we have enough primes: we need p_{n-1} and p_{n+1} */
    generate_primes_up_to((idx_t)(n + 1));

    /* Retrieve the (n-1)-th and (n+1)-th primes (1‑based indexing) */
    uint64_t p_prev = (uint64_t)prime_list[(size_t)(n - 2)];  /* p_{n-1} */
    uint64_t p_next = (uint64_t)prime_list[(size_t)(n)];      /* p_{n+1} */

    uint64_t low  = p_prev * p_prev + 1ull;
    uint64_t high = p_next * p_next - 1ull;

    if (low > high) return 0;
    return count_primes_interval(low, high);
}
