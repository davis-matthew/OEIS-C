/*
 * A144775  Define f(n) = 2 * rad(n) if four divides n and rad(n) otherwise:
 *          then a(n) = 0 for composite n where f(n) is not less than n and
 *          otherwise equals the number of positive integers k less than n for
 *          which f(k) < f(n).
 * https://oeis.org/A144775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of k < n with f(k) < f(n), where f(n) = rad(n) if
 *            n mod 4 ≠ 0 = 2 * rad(n) if n mod 4 = 0. If n is composite and
 *            f(n) ≥ n, then a(n) = 0
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   is_prime, seq_cmp, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_cmp.h"

#define MAX_N 20000   /* sufficient for n up to about 10000 */

static idx_t spf[MAX_N + 1];   /* smallest prime factor */
static seq_t rad[MAX_N + 1];   /* radical of each number */

/* Precompute spf array up to limit n.  */
static void compute_spf(idx_t n)
{
    for (idx_t i = 0; i <= n; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= n; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= n; ++i)
        if (spf[i] == i)
            for (idx_t j = i * i; j <= n; j += i)
                if (spf[j] == j) spf[j] = i;
}

/* Precompute rad array up to limit n. */
static void compute_rad(idx_t n)
{
    rad[0] = 0;
    rad[1] = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t p  = spf[i];
        idx_t j  = i / p;
        rad[i]   = rad[j];
        if (spf[j] != p)
            rad[i] = seq_mul(rad[i], (seq_t)p);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 0;

    if (n > MAX_N)
        abort();   /* out of bounds – extend MAX_N if needed */

    compute_spf(n);
    compute_rad(n);

    /* f(n) */
    seq_t f_n = (n & 3ULL) ? rad[n] : seq_mul(rad[n], (seq_t)2);

    /* Check composite condition */
    if (n > 1 && !is_prime(n))
    {
        seq_t n_seq = (seq_t)n;
        if (seq_cmp(&f_n, &n_seq) >= 0) return 0;
    }

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t f_k = (k & 3ULL) ? rad[k] : seq_mul(rad[k], (seq_t)2);
        if (seq_cmp(&f_k, &f_n) < 0)
            ++count;
    }
    return count;
}
