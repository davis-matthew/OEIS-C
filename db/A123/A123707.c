/*
 * A123707  a(n) = Sum_{k=1..n} A123706(n,k)*2^(k-1).
 * https://oeis.org/A123707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A123706(n,k) * 2^(k-1)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mu_of.h"

/* Local cache for powers of 2 */
static seq_t *pow_cache = NULL;
static idx_t pow_cache_size = 0;

/* Ensure pow_cache has entries up to 'needed' inclusive */
static void ensure_pow_cache(idx_t needed)
{
    if (needed < pow_cache_size) return;
    size_t new_size = (size_t)needed + 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) { /* allocation failure: abort */
        abort();
    }
    /* copy existing contents */
    for (idx_t i = 0; i < pow_cache_size; ++i) {
        new_cache[i] = pow_cache[i];
    }
    /* compute new entries */
    if (pow_cache_size == 0) {
        new_cache[0] = 1;
        for (idx_t i = 1; i < new_size; ++i) {
            new_cache[i] = new_cache[i - 1] * 2;
        }
    } else {
        for (idx_t i = pow_cache_size; i < new_size; ++i) {
            new_cache[i] = new_cache[i - 1] * 2;
        }
    }
    free(pow_cache);
    pow_cache = new_cache;
    pow_cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* out of range, return 0 */

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* Prepare powers of 2 up to exponent (n/d - 2) for the largest possible d=1 */
    if (n >= 2) ensure_pow_cache(n - 2);

    /* Enumerate divisors d of n with d <= n/2 */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;

        /* process divisor d */
        if (d <= n / 2) {
            int mu = mu_of(d);
            if (mu != 0) {
                idx_t exp = n / d - 2;   /* n/d >= 2, so exp >= 0 */
                seq_t val = pow_cache[exp];
                if (mu > 0) sum_pos += val;
                else sum_neg += val;
            }
        }

        /* process complementary divisor e = n/d if distinct and <= n/2 */
        idx_t e = n / d;
        if (e != d && e <= n / 2) {
            int mu = mu_of(e);
            if (mu != 0) {
                idx_t exp = n / e - 2;   /* same as d because e divides n */
                seq_t val = pow_cache[exp];
                if (mu > 0) sum_pos += val;
                else sum_neg += val;
            }
        }
    }

    /* contribution from mu(n) (d = n) */
    int mu_n = mu_of(n);
    if (mu_n > 0)      sum_pos += 1;
    else if (mu_n < 0) sum_neg += 1;

    /* Final result: sum_pos - sum_neg (guaranteed non‑negative) */
    return sum_pos - sum_neg;
}
