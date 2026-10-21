/*
 * A074374  a(n) = sopfr(n)*(sopfr(n)+1)/2 where sopfr is the sum of the prime
 *          factors of n with repetition (A001414).
 * https://oeis.org/A074374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sopfr(n)*(sopfr(n)+1)/2, where sopfr is the sum of *
 *            prime factors of n with multiplicity (A001414)
 * Range:     u8 n <= 22, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* dynamic smallest‑prime‑factor array */
static size_t spf_capacity = 0;
static uint32_t *spf = NULL;

/* enlarge the SPF array to cover 0…max_n */
static void
ensure_spf(size_t max_n)
{
    if (max_n < spf_capacity)
        return;
    size_t new_cap = max_n + 1;
    uint32_t *old = spf;
    spf = (uint32_t *)realloc(old, new_cap * sizeof(uint32_t));
    if (!spf) {
        free(old);
        abort();
    }
    for (size_t i = spf_capacity; i <= max_n; ++i)
        spf[i] = (uint32_t)i;                 /* default: itself */
    for (size_t i = 2; (uint64_t)i * i <= max_n; ++i) {
        if (spf[i] == (uint32_t)i)           /* i is prime */
            for (size_t j = i * i; j <= max_n; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;    /* store smallest prime factor */
    }
    spf_capacity = new_cap;
}

/* sum of prime factors of n, with repetition */
static seq_t
sopfr_value(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    seq_t sum = 0;
    uint64_t x = n;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        sum += (seq_t)p;
        x /= p;
    }
    return sum;
}

/* required OEIS sequence function */
seq_t
a(idx_t n)
{
    ensure_spf((size_t)n);
    seq_t s = sopfr_value(n);
    return s * (s + 1) / 2;
}
