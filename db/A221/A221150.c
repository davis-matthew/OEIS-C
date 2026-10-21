/*
 * A221150  The generalized Fibonacci word f^[3].
 * https://oeis.org/A221150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor((n + 2) / (phi + 2)) - floor((n + 1) / (phi + 2))
 * where phi = (1 + sqrt(5)) / 2.
 *
 * The implementation uses the Beatty representation:
 *    a(n) = 1 if and only if n + 1 is in the Beatty sequence
 *          floor(k * beta) with beta = (5 + sqrt(5)) / 2.
 *
 * For an irrational number alpha, the convergents p/q of its continued
 * fraction satisfy
 *    floor(m * alpha) = floor(m * p / q)   for all m < q.
 *
 * The continued fraction of r = 1/(phi+2) = (5 - sqrt(5))/10 is
 *    r = [0; 3, 1, 1, 1, ...].
 * Hence the convergents are produced by Fibonacci-like recurrence:
 *
 *      p0 = 0, q0 = 1       (a0 = 0)
 *      p1 = 1, q1 = 3       (a1 = 3)
 *      for k >= 2:  pk = pk-1 + pk-2
 *                   qk = qk-1 + qk-2
 *
 * For a requested n we find the first convergent with denominator qk
 * greater than n + 2.  The floor values are then computed exactly
 * using integer arithmetic with 128‑bit intermediates.
 */

#include <stdint.h>
#include <stddef.h>

/* Ensure static state is internal to this file */
static uint64_t *conv_num = NULL;
static uint64_t *conv_den = NULL;
static size_t conv_len = 0;
static size_t conv_cap = 0;

/* Grow the convergent tables to accommodate at least new_cap elements */
static void grow_conv(size_t new_cap)
{
    if (conv_cap >= new_cap) return;
    size_t new_size = new_cap * 2;
    uint64_t *n = (uint64_t *)realloc(conv_num, new_size * sizeof(uint64_t));
    uint64_t *d = (uint64_t *)realloc(conv_den, new_size * sizeof(uint64_t));
    if (!n || !d) { /* Allocation failed – abort */ }
    conv_num = n;
    conv_den = d;
    conv_cap = new_size;
}

/* Generate convergents until denominator > target (i.e. > n+2) */
static void ensure_convergent(idx_t n)
{
    idx_t target = n + 2;
    if (conv_len == 0) {
        /* Initialize first two convergents */
        grow_conv(8);
        conv_num[0] = 0; conv_den[0] = 1;
        conv_num[1] = 1; conv_den[1] = 3;
        conv_len = 2;
    }
    while (conv_len > 0 && conv_den[conv_len - 1] <= (uint64_t)target) {
        /* Next term of the continued fraction is always 1 for r */
        uint64_t a = conv_num[conv_len - 1] + conv_num[conv_len - 2];
        uint64_t b = conv_den[conv_len - 1] + conv_den[conv_len - 2];
        grow_conv(conv_len + 1);
        conv_num[conv_len] = a;
        conv_den[conv_len] = b;
        conv_len++;
    }
}

seq_t a(idx_t n)
{
    /* Find convergent whose denominator exceeds n+2 */
    ensure_convergent(n);

    /* Use the last convergent (denominator > n+2) */
    size_t idx = conv_len - 1;
    uint64_t num = conv_num[idx];
    uint64_t den = conv_den[idx];

    /* Compute floor((n+2)*num/den) and floor((n+1)*num/den) with 128-bit intermediates */
    unsigned __int128 t1 = (unsigned __int128)(n + 2) * (unsigned __int128)num;
    unsigned __int128 t2 = (unsigned __int128)(n + 1) * (unsigned __int128)num;
    seq_t v1 = (seq_t)(t2 / den);      /* floor((n+1)*r) */
    seq_t v2 = (seq_t)(t1 / den);      /* floor((n+2)*r) */

    return v2 - v1;   /* 0 or 1 */
}
