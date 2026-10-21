/*
 * A054201  a(n) = (n-1)! * Sum_{k=1..n} k^k/k!.
 * https://oeis.org/A054201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = (n-1)*a(n-1) + n^(n-1) for n≥2 (equivalent to
 *            a(n) = (n-1)! * sum_{k=1..n} k^k/k! )
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Internal helper: integer exponentiation in the seq_t type. */
static seq_t pow_seq_uint(idx_t base, idx_t exp)
{
    seq_t result = 1;               /* seq_t(1) */
    seq_t b = (seq_t)base;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) result = result * b;
        b = b * b;
        e >>= 1;
    }
    return result;
}

/* Cache for already computed values. */
static seq_t *a_cache = NULL;      /* array of computed a(k) for k ≥ 1 */
static idx_t a_len  = 0;           /* number of stored elements (k = 1..a_len) */
static idx_t a_cap  = 0;           /* allocated capacity of a_cache */

/* Reallocate cache if needed. */
static void ensure_a_capacity(idx_t need)
{
    if (need <= a_cap) return;
    idx_t new_cap = (a_cap == 0) ? 1 : a_cap;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = tmp;
    a_cap = new_cap;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range: conventionally 0 */
    /* Ensure cache covers n. */
    if (n > a_len) {
        ensure_a_capacity(n);
        /* Compute new values up to n. */
        for (idx_t i = a_len + 1; i <= n; ++i) {
            seq_t term;
            if (i == 1) {
                term = 1;                 /* a(1) = 1 */
            } else {
                /* (i-1)*a(i-1) + i^(i-1) */
                seq_t mult = a_cache[i-2] * (seq_t)(i-1);
                seq_t power = pow_seq_uint((idx_t)i, (idx_t)(i-1));
                term = mult + power;
            }
            a_cache[i-1] = term;
        }
        a_len = n;
    }
    return a_cache[n-1];
}
