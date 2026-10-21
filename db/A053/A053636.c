/*
 * A053636  a(n) = Sum_{odd d|n} phi(d)*2^(n/d).
 * https://oeis.org/A053636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{odd d|n} phi(d) * 2^(n/d)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"

/* Memoisation of Euler's totient function up to the largest n requested. */
static uint64_t *phi_cache_uint64 = NULL;
static idx_t phi_cap = 0;

/* Compute φ(k) for all 0 ≤ k ≤ n, using the simple sieve method. */
static void ensure_phi(idx_t n) {
    if (phi_cache_uint64 && n <= phi_cap)
        return;

    size_t cap = (size_t)n + 1;
    uint64_t *tmp = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!tmp)
        return;             /* out of memory – unlikely in the test environment */

    for (size_t i = 0; i < cap; ++i)
        tmp[i] = i;          /* φ(i) starts as i */

    for (size_t p = 2; p <= (size_t)n; ++p) {
        if (tmp[p] == p) {   /* p is prime */
            for (size_t j = p; j <= (size_t)n; j += p)
                tmp[j] -= tmp[j] / p;
        }
    }

    free(phi_cache_uint64);
    phi_cache_uint64 = tmp;
    phi_cap = n;
}

/* Return a(n) for the OEIS sequence A053636.  External linkage is required. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                 /* a(0) = 0 */

    ensure_phi(n);                /* need φ(k) up to n */

    /* Extract the odd part of n (all odd divisors of n are divisors of this). */
    idx_t odd_part = n;
    while ((odd_part & 1ULL) == 0)
        odd_part >>= 1;

    seq_t sum = 0;
    /* Iterate over odd divisors d of odd_part (hence odd divisors of n). */
    for (idx_t d = 1; d <= odd_part; d += 2) {
        if (odd_part % d == 0) {
            seq_t phi_d = (seq_t)phi_cache_uint64[d];
            seq_t pow_term = pow2(n / d);  /* 2^(n/d) */
            sum += phi_d * pow_term;
        }
    }
    return sum;
}
