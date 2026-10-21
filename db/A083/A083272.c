/*
 * A083272  a(n) = n*tau(a(n-1)) + 1 = n*A000005(a(n-1)) + 1, a(0) = 1.
 * https://oeis.org/A083272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * tau(a(n-1)) + 1, a(0) = 1 where tau(m) = number of
 *            divisors of m
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use a lazy cache; compute each value only once.  */
#include <stdint.h>
#include <stdlib.h>

/* --------- helper: divisor count for uint64_t -------------------- */
static uint64_t divisor_count_u64(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t d = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            d *= (e + 1);
        }
    }
    if (m > 1) d *= 2;          /* remaining prime factor */
    return d;
}
/* --------------------------------------------------------------- */

/* --------------------------------------------------------------- */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *ptr = tmp;
    *cap = new_cap;
}
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* cache[0] = 1 ; cache[i] = a(i) */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_computed = (idx_t)(-1);

    /* Allocate space for n+1 entries */
    ensure_capacity((size_t)n + 1, &cap, &cache);

    /* Initialise a(0) if this is the first call */
    if (last_computed == (idx_t)(-1)) {
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* Compute missing values */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        uint64_t tau = divisor_count_u64((uint64_t)prev);
        cache[i] = (seq_t)i * (seq_t)tau + (seq_t)1;
    }
    last_computed = n;

    return cache[n];
}
