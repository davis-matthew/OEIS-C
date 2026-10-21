/*
 * A071221  Product of next n composite numbers.
 * https://oeis.org/A071221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the next n composite numbers. Computed by
 *            sieving to determine compositeness and multiplying the required
 *            block of composite numbers
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  Static sieve data  ---------- */
static uint8_t *g_is_prime = NULL;   /* 0 : composite, 1 : prime */
static size_t  g_sieve_limit = 0;    /* current sieve upper bound */

/* ----------  Eratosthenes sieve  ---------- */
static void rebuild_sieve(size_t limit)
{
    if (g_is_prime) free(g_is_prime);

    g_is_prime = (uint8_t*)calloc(limit + 1, 1);
    if (!g_is_prime) exit(EXIT_FAILURE);

    /* treat all >=2 as prime initially */
    for (size_t i = 2; i <= limit; ++i) g_is_prime[i] = 1;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (g_is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                g_is_prime[j] = 0;
        }
    }
    g_sieve_limit = limit;
}

/* Count composites up to the current sieve limit */
static size_t composite_count(void)
{
    if (!g_is_prime) return 0;
    size_t cnt = 0;
    for (size_t i = 4; i <= g_sieve_limit; ++i)
        if (!g_is_prime[i]) ++cnt;
    return cnt;
}

/* Ensure enough composites for the range [1 .. target] */
static void ensure_sieve_for_composites(size_t target)
{
    if (!g_is_prime) {
        rebuild_sieve(target * 2 + 10);
        if (composite_count() < target) {
            size_t limit = g_sieve_limit * 2;
            while (composite_count() < target) limit *= 2;
            rebuild_sieve(limit);
        }
        return;
    }

    if (composite_count() >= target) return;

    size_t limit = g_sieve_limit * 2;
    while (1) {
        rebuild_sieve(limit);
        if (composite_count() >= target) break;
        limit *= 2;
    }
}

/* ----------  Sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used for the OEIS sequence */

    /* Composite index range for this term */
    size_t start = (size_t)n * (n - 1) / 2 + 1;
    size_t end   = (size_t)n * (n + 1) / 2;

    /* Build sieve large enough to contain at least 'end' composites */
    ensure_sieve_for_composites(end);

    size_t comp_idx = 0;          /* current composite index */
    seq_t prod = (seq_t)1;        /* running product */

    for (size_t num = 4; ; ++num) {
        if (num > g_sieve_limit) break;              /* safety guard */
        if (!g_is_prime[num]) {
            ++comp_idx;
            if (comp_idx > end) break;               /* finished needed block */
            if (comp_idx >= start)
                prod = prod * (seq_t)num;            /* multiply into product */
        }
    }
    return prod;
}
