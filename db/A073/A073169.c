/*
 * A073169  a(n)=A002808(n)-n, difference between n-th composite and n.
 * https://oeis.org/A073169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002808(n) – n, i.e., the n‑th composite number minus n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static idx_t limit = 0;
static bool *is_prime = NULL;
static idx_t *pi_arr = NULL;

/* Sieve of Eratosthenes up to new_limit and compute prime prefix counts pi_arr */
static void build_sieve(idx_t new_limit)
{
    is_prime = (bool*)realloc(is_prime, (size_t)(new_limit + 1) * sizeof(bool));
    pi_arr   = (idx_t *)realloc(pi_arr,   (size_t)(new_limit + 1) * sizeof(idx_t));

    for (idx_t i = 0; i <= new_limit; ++i) is_prime[i] = true;
    if (new_limit >= 0) is_prime[0] = false;
    if (new_limit >= 1) is_prime[1] = false;

    for (idx_t p = 2; p * p <= new_limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= new_limit; m += p)
                is_prime[m] = false;
        }
    }

    idx_t cnt = 0;
    for (idx_t i = 0; i <= new_limit; ++i) {
        if (is_prime[i]) ++cnt;
        pi_arr[i] = cnt;
    }
    limit = new_limit;
}

/* Ensure that the sieve is large enough so that the n‑th composite exists in
   the current range. Expands the sieve twice until enough composites are
   covered. */
static void ensure_limit(idx_t n)
{
    if (limit == 0) build_sieve(200);   /* initial factor for small n */

    while ((limit - 1) - pi_arr[limit] < n) {
        idx_t new_limit = limit * 2;
        if (new_limit <= limit) new_limit = limit + 1000;   /* overflow guard */
        build_sieve(new_limit);
    }
}

/* External function requested by the framework */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; behaviour below is undefined */

    ensure_limit(n);

    /* Binary search for smallest m such that (m-1) - pi(m) >= n */
    idx_t lo = 2, hi = limit;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t comp = (mid - 1) - pi_arr[mid];
        if (comp >= n) hi = mid;
        else lo = mid + 1;
    }
    seq_t composite = lo;
    return composite - (seq_t)n;  /* difference between n-th composite and n */
}
