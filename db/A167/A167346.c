/*
 * A167346  Totally multiplicative sequence with a(p) = (p-1)*(p+2) = p^2+p-2
 *          for prime p.
 * https://oeis.org/A167346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = \prod_{p^e||n} (p^2+p-2)^e (totally multiplicative with
 *            a(p) = p^2+p-2)
 * Range:     u8 n <= 14, u16 n <= 224, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* idx_t and seq_t are provided by the build system. */
extern seq_t a(idx_t n);

/* static cache for values and smallest prime factors */
static seq_t *a_cache = NULL;          /* a_cache[i] == a(i) */
static idx_t cached_limit = 0;         /* largest n cached */
static idx_t *spf_cache = NULL;        /* spf_cache[i] == smallest prime factor of i */
static idx_t spf_limit = 0;            /* largest n for which spf_cache is valid */

/* simple sieve to fill spf_cache up to limit */
static void compute_spf(idx_t limit, idx_t *spf)
{
    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
    if (limit >= 0) spf[0] = 0;
    if (limit >= 1) spf[1] = 1;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {               /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
}

/* Ensure that arrays cover up to max_needed. */
static void ensure_cache(idx_t max_needed)
{
    if (max_needed <= cached_limit) return;

    /* Reallocate a_cache */
    seq_t *new_a = (seq_t *)realloc(a_cache, (max_needed + 1) * sizeof(seq_t));
    if (!new_a) { /* allocation failure; abort silently */
        return;
    }
    a_cache = new_a;

    /* Reallocate spf_cache if necessary */
    if (spf_cache == NULL || spf_limit < max_needed) {
        idx_t *new_spf = (idx_t *)realloc(spf_cache, (max_needed + 1) * sizeof(idx_t));
        if (!new_spf) {
            return;
        }
        spf_cache = new_spf;
        compute_spf(max_needed, spf_cache);
        spf_limit = max_needed;
    }

    /* Compute a(i) for i from cached_limit+1 to max_needed */
    for (idx_t i = cached_limit + 1; i <= max_needed; ++i) {
        if (i == 1) {
            a_cache[1] = 1;
            continue;
        }

        idx_t p = spf_cache[i];
        idx_t temp = i;
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* a(p) = p^2 + p - 2 */
        seq_t ap = (seq_t)p * (seq_t)p + (seq_t)p - 2;

        /* compute ap^e */
        seq_t pow_val = 1;
        for (idx_t k = 0; k < e; ++k) pow_val *= ap;

        /* a(i) = ap^e * a(temp) */
        a_cache[i] = pow_val * a_cache[temp];
    }

    cached_limit = max_needed;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined, just return 0 */
    ensure_cache(n);
    return a_cache[n];
}
