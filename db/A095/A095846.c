/*
 * A095846  Expansion of eta(q^2)eta(q^10)^3/(eta(q^5)eta(q)^3) in powers of
 *          q.
 * https://oeis.org/A095846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence n * a(n) = Σ_{k=1}^{n} S(k) *
 *            a(n-k), for n ≥ 1 with a(1) = 1, where S(k) = Σ_{d|k} d * b(d)
 *            and b(d) = 3 - (d even ? 1 : 0) - 3*(d%10==0) + (d%5==0)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external libraries are used beyond std headers. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* External integer types supplied by the build. */
extern seq_t a(idx_t n);

/* Static caches for the recurrence. */
static seq_t *B = NULL;          /* B[0..cap-1]  (shifted: a(n) = B[n-1]) */
static seq_t *S = NULL;          /* S[0..cap-1] (S[0] unused)                */
static size_t  cap = 0;          /* allocated capacity                   */
static size_t  computed = 0;     /* highest n for which B[n] is computed (∈[0,cap-1]) */

/* Ensure array storage is large enough for indices up to N inclusive. */
static void ensure_capacity(size_t N)
{
    if (cap <= N) {
        size_t new_cap = (cap == 0) ? 16 : cap << 1;
        if (new_cap <= N) new_cap = N + 1;
        B = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
        S = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
        for (size_t i = cap; i < new_cap; ++i) {
            B[i] = 0;
            S[i] = 0;
        }
        cap = new_cap;
    }
    /* B[0] must be 1 (a(1)).  Initialise it if it has not yet been set. */
    if (cap > 0 && B[0] == (seq_t)0) B[0] = (seq_t)1;
}

/* Recompute all divisor sum values S[k] for 1 ≤ k ≤ N. */
static void recompute_S(size_t N)
{
    /* Zero out the needed range. */
    for (size_t i = 1; i <= N; ++i) S[i] = (seq_t)0;

    for (size_t d = 1; d <= N; ++d) {
        int even   = (d & 1) == 0;
        int div10  = (d % 10) == 0;
        int div5   = (d % 5) == 0;
        seq_t bd = (seq_t)(3 - even - 3*div10 + div5);   /* b(d) */
        seq_t term = (seq_t)d * bd;                      /* d * b(d) */
        for (size_t m = d; m <= N; m += d) {
            S[m] += term;
        }
    }
}

/* Recompute all B[n] values for n = computed+1 .. N. */
static void recompute_B(size_t N)
{
    /* B[0] is already set to 1 by ensure_capacity. */
    for (size_t n = computed + 1; n <= N; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; k <= n; ++k) {
            sum += S[k] * B[n - k];
        }
        B[n] = sum / (seq_t)n;   /* division is guaranteed exact */
    }
    computed = N;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset; safe fallback */
    /* a(1) corresponds to B[0] */
    if (n == 1) return (seq_t)1;

    size_t idx = (size_t)(n - 1);         /* use B[n-1] */
    ensure_capacity(idx);
    recompute_S(idx);                     /* need S[1..idx] */
    if (computed < idx) recompute_B(idx); /* compute B up to idx */

    return B[idx];
}
