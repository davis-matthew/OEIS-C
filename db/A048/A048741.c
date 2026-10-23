/*
 * A048741  Product of aliquot divisors of composite n (1 and primes omitted).
 * https://oeis.org/A048741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A007956(A002808(n))
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 64, u64 n <= 186, u128 n <= 590,
 *            big any n
 * Helpers:   factor_u64, int_sqrt, is_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = product of all proper divisors of the n‑th composite number */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/int_sqrt.h"
#include "../utils/seq_pow.h"

/* cache of composite numbers */
static idx_t *composite_cache = NULL;
static size_t composite_cap   = 0;
static size_t composite_len   = 0;

/* ensure the cache can hold at least `needed` elements */
static void ensure_composite_capacity(size_t needed) {
    if (needed <= composite_cap) return;
    size_t new_cap = composite_cap ? composite_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(composite_cache, new_cap * sizeof(idx_t));
    composite_cache = tmp;
    composite_cap   = new_cap;
}

/* grow the cache until it contains the `target`‑th composite */
static void extend_composite_cache_to(idx_t target) {
    if (composite_len >= target) return;
    idx_t start = composite_len ? composite_cache[composite_len - 1] + 1 : 4;
    for (idx_t i = start; composite_len < target; ++i) {
        if (!is_prime(i)) {
            ensure_composite_capacity(composite_len + 1);
            composite_cache[composite_len++] = i;
        }
    }
}

/* product of all proper (aliquot) divisors of m */
static seq_t product_of_aliquot_divisors(idx_t m) {
    /* factor m */
    uint64_t n_val = (uint64_t)m;          /* fits in 64 bits for this range */
    uint64_t pr[64], ex[64];
    size_t   cnt = 0;
    factor_u64(n_val, pr, ex, &cnt);

    /* τ(m) = number of divisors */
    idx_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) tau *= (idx_t)(ex[i] + 1);

    if ((tau & 1) == 0) {                 /* τ is even */
        idx_t exp = tau / 2 - 1;
        return seq_pow((seq_t)n_val, exp);
    } else {                               /* τ odd → m is a perfect square */
        idx_t e_int = (tau - 3) / 2;       /* integer part of the exponent */
        seq_t part  = seq_pow((seq_t)n_val, e_int);
        seq_t sqrt  = (seq_t)int_sqrt(n_val);  /* exact square root */
        return part * sqrt;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined, but safe */
    extend_composite_cache_to(n);
    idx_t composite = composite_cache[n - 1];
    return product_of_aliquot_divisors(composite);
}
