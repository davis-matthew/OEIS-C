/*
 * A068773  Alternating sum phi(1) - phi(2) + phi(3) - phi(4) + ... +
 *          ((-1)^(n+1))*phi(n).
 * https://oeis.org/A068773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1)*phi(k)
 * Range:     u8 n <= 48, u16 n <= 802, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *alt = NULL;          /* alternating sums, alt[0] = 0        */
static idx_t  alt_max = 0;         /* current maximum precalculated index */

/* Recompute alternating sums up to n (inclusive).  */
static void ensure_alt_upto(idx_t n)
{
    if (n <= alt_max)
        return;

    if (n == (idx_t)0) {
        /* nothing to do */
        return;
    }

    /* Allocate temporary array for phi values */
    size_t sz = (size_t)(n + 1);
    seq_t *phi = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!phi)
        return;  /* allocation failed – leave alt unchanged */

    /* Initialise phi[i] = i */
    for (idx_t i = 1; i <= n; ++i)
        phi[i] = (seq_t)i;

    /* Sieve to compute phi values */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == (seq_t)p) {      /* p is prime */
            for (idx_t j = p; j <= n; j += p)
                phi[j] -= phi[j] / (seq_t)p;
        }
    }

    /* Allocate new alternating‑sum array */
    seq_t *new_alt = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_alt) {
        free(phi);
        return;
    }

    new_alt[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (i & 1U)               /* odd i  →  +phi(i)  */
            new_alt[i] = new_alt[i - 1] + phi[i];
        else                       /* even i →  -phi(i)  */
            new_alt[i] = new_alt[i - 1] - phi[i];
    }

    free(phi);

    /* Replace old table */
    if (alt)
        free(alt);
    alt      = new_alt;
    alt_max  = n;
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0;

    ensure_alt_upto(n);
    return alt ? alt[n] : (seq_t)0;
}
