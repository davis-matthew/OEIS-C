/*
 * A115978  Expansion of phi(-q) * phi(-q^3) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A115978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2 * b(n), where b(n) is a multiplicative function with
 *            b(1) = 1 and for n = p^e (p prime) : * if p == 2 : b(2^e) = -3
 *            if e even, 0 if e odd * if p == 3 : b(3^e) = 1 * if p % 6 == 1 :
 *            b(p^e) = 1 + e * if p % 6 == 5 : b(p^e) = 1 if e even, 0 if e
 *            odd For composite n, b(n) = Π b(p_i^e_i). Th...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static idx_t *spf = NULL;          /* smallest prime factor array   */
static size_t spf_size = 0;        /* size of spf array (n+1)       */

/* Ensure that spf[0..n] is initialized with smallest prime factors.  */
static void ensure_spf(idx_t n)
{
    if (spf != NULL && n < spf_size) return;

    size_t sz = (size_t)n + 1;
    idx_t *new_spf = (idx_t *)realloc(spf, sz * sizeof(idx_t));
    if (!new_spf) { abort(); }            /* allocation failure          */
    spf = new_spf;

    /* initialize the array to 0 */
    for (size_t i = 0; i <= n; ++i) spf[i] = 0;

    /* Sieve for smallest prime factors */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i > n / i) continue;
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_size = sz;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 by definition       */

    ensure_spf(n);
    seq_t prod = 1;
    idx_t m = n;

    while (m > 1) {
        idx_t p = spf[m];
        if (!p) p = m;             /* fallback for a prime > array */

        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        seq_t factor;
        if (p == 2) {
            factor = (exp % 2 == 0) ? (-3) : 0;
        } else if (p == 3) {
            factor = 1;
        } else if (p % 6 == 1) {
            factor = (seq_t)exp + 1;
        } else { /* p % 6 == 5 */
            factor = (exp % 2 == 0) ? 1 : 0;
        }

        prod *= factor;
        if (prod == 0) break;
    }

    return -2 * prod;
}
