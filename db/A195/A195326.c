/*
 * A195326  Numerators of fractions leading to e - 1/e (A174548).
 * https://oeis.org/A195326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   * D[0] = 0; * D[n] = n * D[n-1] + 2*(n mod 2) (for n >= 1) *
 *            a(n) = D[n] / gcd( D[n], n! ) (for n >= 0) * (where n! is the
 *            factorial of n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *D = NULL;         /* array of D[n] values */
static seq_t *fact = NULL;      /* array of n! values */
static idx_t cached_n = 0;

/* Ensure that arrays are computed up to index n (inclusive). */
static void ensure_up_to(idx_t n)
{
    if (n <= cached_n)
        return;

    size_t new_size = (size_t)n + 1;  /* indices 0..n */
    D = (seq_t*)realloc(D, new_size * sizeof(seq_t));
    fact = (seq_t*)realloc(fact, new_size * sizeof(seq_t));
    if (!D || !fact)
        exit(EXIT_FAILURE);   /* allocation failed */

    /* Initialize base case if first time */
    if (cached_n == 0) {
        D[0] = (seq_t)0;
        fact[0] = (seq_t)1;
    }

    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* fact[i] = i! */
        fact[i] = fact[i-1] * (seq_t)i;

        /* D[i] = i * D[i-1] + 2 if i is odd, else i * D[i-1] */
        seq_t mul = D[i-1] * (seq_t)i;
        seq_t add = (i & 1U) ? (seq_t)2 : (seq_t)0;
        D[i] = mul + add;
    }

    cached_n = n;
}

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* a(0) = 0 */

    ensure_up_to(n);

    /* Compute reduced numerator: D[n] / gcd(D[n], fact[n]) */
    seq_t g = gcd(D[n], fact[n]);
    return D[n] / g;
}
