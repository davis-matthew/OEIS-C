/*
 * A064749  a(n) = n*11^n + 1.
 * https://oeis.org/A064749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*11^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached powers of 11; pow11[i] = 11^i */
static seq_t *pow11 = NULL;
static idx_t pow11_len = 0;

/* Grow the cache to contain 11^n */
static void
ensure_pow11(idx_t n)
{
    if (pow11_len > n)
        return;                    /* already computed */

    idx_t new_len = n + 1;
    seq_t *new_pow11 = (seq_t *)realloc(pow11, new_len * sizeof(seq_t));
    if (!new_pow11) {
        /* Out of memory – terminate; this should never happen in normal use */
        abort();
    }
    pow11 = new_pow11;

    if (pow11_len == 0) {
        /* First element */
        pow11[0] = (seq_t)1;
        pow11_len = 1;
    }

    for (idx_t i = pow11_len; i < new_len; ++i) {
        /* pow11[i] = pow11[i-1] * 11 */
        pow11[i] = pow11[i-1] * 11;
    }
    pow11_len = new_len;
}

seq_t a(idx_t n)
{
    /* Ensure we have 11^n */
    ensure_pow11(n);
    /* a(n) = n * 11^n + 1 */
    seq_t res = ((seq_t)n * pow11[n]) + 1;
    return res;
}
