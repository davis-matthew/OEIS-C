/*
 * A115119  Number of imprimitive (periodic) n-bead necklaces with beads of 2
 *          colors when turning over is allowed.
 * https://oeis.org/A115119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of imprimitive binary bracelets of length n * =
 *            A000029(n) - A001371(n) * where A000029(n) = total binary
 *            bracelets, A001371(n) = primitive binary bracelets
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   calc_phi, mu_of, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"          /* pow2(idx_t exp) → seq_t */
#include "../utils/calc_phi.h"      /* calc_phi(seq_t m) → φ(m) */
#include "../utils/mu_of.h"         /* mu_of(idx_t d) → Möbius value (-1,0,1) */

/* Memoised values of A000029(n) (binary bracelets) */
static seq_t *B_cache = NULL;
static idx_t B_cache_cap = 0;

/* Ensure the cache can hold values up to `limit`. */
static void ensure_B_cache(idx_t limit)
{
    if (limit <= B_cache_cap)
        return;
    size_t new_cap = (size_t)limit + 1;
    seq_t *new_buf = (seq_t *)realloc(B_cache, new_cap * sizeof(seq_t));
    if (!new_buf)
        exit(1);  /* allocation failure */
    for (idx_t i = B_cache_cap; i <= limit; ++i)
        new_buf[i] = 0;
    B_cache = new_buf;
    B_cache_cap = limit;
}

/* Compute A000029(n): number of binary bracelets of length n. */
static seq_t compute_B(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_B_cache(n);
    if (B_cache[n] != 0)
        return B_cache[n];

    seq_t sum = 0;

    /* Sum over divisors d of n: φ(d) * 2^(n/d) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d)
            continue;

        idx_t d1 = d;
        seq_t phi1 = calc_phi((seq_t)d1);
        seq_t pow1 = pow2(n / d1);
        sum += pow1 * phi1;

        idx_t d2 = n / d;
        if (d2 == d1)
            continue;
        seq_t phi2 = calc_phi((seq_t)d2);
        seq_t pow2_ = pow2(n / d2);
        sum += pow2_ * phi2;
    }

    /* Add reflection term */
    seq_t ref;
    if (n & 1) {                      /* n odd */
        ref = (seq_t)n * pow2((n + 1) / 2);
    } else {                           /* n even */
        seq_t h = pow2(n / 2);
        seq_t h_plus = pow2(n / 2 + 1);
        ref = ((seq_t)(n / 2)) * (h + h_plus);
    }
    sum += ref;

    seq_t denom = (seq_t)(2 * n);
    seq_t B = sum / denom;

    B_cache[n] = B;   /* memoise */
    return B;
}

/* Public function: a(idx_t n) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t result = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d)
            continue;

        if (d > 1) {
            seq_t B_val = compute_B(n / d);
            result += (seq_t)(-mu_of(d)) * B_val;
        }

        idx_t d2 = n / d;
        if (d2 != d && d2 > 1) {
            seq_t B_val = compute_B(n / d2);   /* n/d2 == d */
            result += (seq_t)(-mu_of(d2)) * B_val;
        }
    }

    return result;
}
