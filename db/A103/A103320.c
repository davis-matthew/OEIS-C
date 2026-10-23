/*
 * A103320  Condensed Golomb sequence: digits according to Golomb's sequence,
 *          same digits concatenated.
 * https://oeis.org/A103320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, fini, full
 * Formula:   a(n) = the decimal digit 'n' repeated G(n) times, where G is the
 *            Golomb sequence (G(1)=1, G(n)=1+G(n-G(G(n-1)))). The function
 *            returns the concatenation as a seq_t value
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, pow10_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/pow10_idx.h"

/* Golomb sequence cached up to the largest index requested. */
static idx_t *Golomb = NULL;     /* 1‑indexed, Golomb[1]=1 */
static idx_t G_len   = 0;        /* largest index computed */
static idx_t G_cap   = 0;        /* allocated capacity */

/* Ensure Golomb[k] has been computed for all k ≤ n. */
static void ensure_G(idx_t n)
{
    if (n <= G_len) return;                  /* already available */

    /* Grow capacity to at least n+1 (carry an unused slot 0). */
    idx_t new_cap = G_cap > 0 ? G_cap : 16;
    while (new_cap <= n) new_cap <<= 1;

    Golomb = (idx_t *)realloc(Golomb, new_cap * sizeof(idx_t));
    G_cap = new_cap;

    /* Initialise the first term if we are starting from scratch. */
    if (G_len == 0) {
        Golomb[1] = 1;
        G_len = 1;
    }

    /* Compute terms from G_len+1 up to n. */
    for (idx_t i = G_len + 1; i <= n; ++i) {
        idx_t prev     = Golomb[i - 1];
        idx_t idx_val  = i - Golomb[prev];
        Golomb[i] = 1 + Golomb[idx_val];
    }
    G_len = n;
}

/* Return a(n) for the Condensed Golomb sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range, return 0 */

    ensure_G(n);                         /* make sure G[n] is ready */

    idx_t k  = Golomb[n];                /* number of repetitions */
    idx_t d  = digit_len((seq_t)n);      /* number of decimal digits of n */
    seq_t pow10 = pow10_idx(d);          /* 10^d */
    seq_t result = 0;

    for (idx_t i = 0; i < k; ++i)
        result = result * pow10 + (seq_t)n;

    return result;
}
