/*
 * A082299  Greatest common divisor of n and its sum of prime factors (with
 *          repetition).
 * https://oeis.org/A082299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A001414(n)) – the GCD of n and the sum of its *
 *            prime factors (with multiplicity). a(1) = 1, a(2) = 2, a(6) = 1,
 *            …
 * Range:     u8 n <= 256, u16 n <= 9342, u32 n <= 9342, u64 n <= 9342,
 *            u128 n <= 9342, big any n
 * Helpers:   build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/build_spf.h"

/* Smallest‑prime‑factor table (0..max_n).  It is rebuilt only when a larger
 * n is requested. */
static idx_t *spf      = NULL;   /* 0..max_n */
static idx_t  max_n    = 0;      /* largest n for which spf is valid */

/* Ensure we have an SPF table covering all indices up to n. */
static void
ensure_cache(idx_t n)
{
    if (n <= max_n) return;          /* already covered */

    /* Replace the old table if it exists.  Freeing NULL is safe. */
    if (spf) free(spf);

    /* Allocate a new SPF table up to n (inclusive). */
    build_spf(n, &spf);
    max_n = n;
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset < 1, harmless */

    ensure_cache(n);

    /* Compute the sum of prime factors of n. */
    seq_t sum = 0;
    idx_t  m   = n;
    while (m > 1) {
        idx_t p = spf[m];
        sum += (seq_t)p;
        m /= p;
    }

    return gcd((seq_t)n, sum);
}
