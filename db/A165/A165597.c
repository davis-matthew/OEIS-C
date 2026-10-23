/*
 * A165597  Partial sums of A165596.
 * https://oeis.org/A165597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = the partial sum of the sign sequence s(k) = ((k|233) *
 *            (k|257)), where (x|p) is the Legendre symbol. Thus a(0)=0,
 *            a(n)=∑_{k=1}^{n} s(k). The sign sequence is periodic with period
 *            59881. The implementation pre‑computes one period of cumulative
 *            sums and then uses it to answer any query in O(1)
 * Range:     i8 n <= 0, i16 n <= 8525, i32 n <= 8525, i64 n <= 8525,
 *            i128 n <= 8525, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

#define PERIOD 59881U            /* 233 × 257 */

static seq_t *prefix   = NULL;   /* prefix[i] = ∑_{k=1}^i s(k), i=0..PERIOD */
static seq_t period_sum = 0;     /* sum over one full period */

/* Legendre symbol (n|p) for odd prime p.
   Uses Euler's criterion: (n|p)=n^((p-1)/2) mod p
   Returns 1, -1 or 0.  The exponent fits in unsigned 64‑bit. */
static inline int legendre(seq_t n, seq_t p, idx_t exp)
{
    if (n == 0) return 0;
    seq_t v = mod_pow(n, exp, p);   /* n^((p-1)/2) mod p */
    if (v == 1)  return 1;
    if (v == p-1) return -1;
    return 0;
}

/* Build the prefix sum table of the sign sequence.  Thread‑safe in
   the sense that the table is built only once. */
static void init_sign_sequence(void)
{
    if (prefix != NULL) return;            /* already built */

#if defined(__cplusplus)
    /* C++: use operator new[] so that constructors are called */
    prefix = new seq_t[PERIOD + 1];
#else
    /* C: malloc returns void*, cast explicitly */
    prefix = (seq_t*)malloc((PERIOD + 1) * sizeof(seq_t));
    if (prefix == NULL) exit(EXIT_FAILURE);
#endif

    prefix[0] = 0;
    for (idx_t i = 1; i <= PERIOD; ++i) {
        /* Legendre symbols for the two primes */
        int s233 = legendre((seq_t)i, 233, 116);   /* (i|233)  */
        int s257 = legendre((seq_t)i, 257, 128);   /* (i|257)  */
        int s = s233 * s257;                      /* product: -1, 0 or 1 */
        prefix[i] = prefix[i-1] + (seq_t)s;
    }
    period_sum = prefix[PERIOD];
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    init_sign_sequence();

    idx_t q = n / PERIOD;     /* number of full periods */
    idx_t r = n % PERIOD;     /* offset within the last period */

    /* a(n) = q·period_sum + prefix[r] */
    return (seq_t)q * period_sum + prefix[r];
}
