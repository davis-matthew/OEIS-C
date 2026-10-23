/*
 * A112659  a(1) = 1. For n >= 2, a(n) = number of earlier terms of the
 *          sequence which are divisible by a different number of distinct
 *          primes than n.
 * https://oeis.org/A112659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) – #{ i < n | ω(a(i)) = ω(n) } where ω(x) is the
 *            number of distinct prime factors of x
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Count distinct prime factors of a 64‑bit integer (trial division). */
static unsigned omega_uint64(uint64_t x)
{
    unsigned cnt = 0;
    if (x <= 1) return 0;
    if ((x & 1ULL) == 0) {
        ++cnt;
        while ((x & 1ULL) == 0) x >>= 1;
    }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            ++cnt;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) ++cnt;
    return cnt;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but keep safe */

    /* Static storage for generated terms and frequencies of ω(a(i)). */
    static seq_t *values      = NULL;
    static idx_t values_cap   = 0;
    static idx_t values_sz    = 0;
    static unsigned freq[64]  = {0};   /* freq[ω] = number of earlier terms with that ω */

    while (values_sz < n) {
        idx_t idx = values_sz + 1;        /* term position (1‑based) */

        seq_t val;
        if (idx == 1) {
            val = (seq_t)1;
        } else {
            unsigned omega_n = omega_uint64((uint64_t)idx);
            idx_t same = 0;
            if (omega_n < 64) same = freq[omega_n];
            val = (seq_t)(values_sz - same);   /* count of earlier terms with different ω */
        }

        /* Resize array if necessary. */
        if (values_sz >= values_cap) {
            idx_t new_cap = values_cap ? values_cap * 2 : 64;
            values = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
            values_cap = new_cap;
        }

        /* Store the new term. */
        values[values_sz] = val;

        /* Update frequency table for ω of this new term. */
        unsigned omega_val = omega_uint64((uint64_t)val);
        if (omega_val < 64) ++freq[omega_val];

        ++values_sz;
    }

    return values[n - 1];
}
