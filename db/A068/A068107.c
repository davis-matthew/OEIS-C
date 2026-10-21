/*
 * A068107  a(n) = n!*Sum_{k=1..n} mu(k)/k!, where mu(k) is the Moebius
 *          function.
 * https://oeis.org/A068107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{k=1..n} mu(k)/k!
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(n)=n*a(n-1)+mu(n) for n≥2. */

#include <stdlib.h>

/* static cache for Moebius values */
static idx_t mu_max = 0;
static signed char *mu_arr = NULL;

/* Ensure mu_arr[1..n] is filled.  Recomputes from scratch up to n. */
static void ensure_mu(idx_t n)
{
    if (n <= mu_max) return;

    idx_t size = n + 1;
    signed char *new_arr = (signed char *)calloc(size, sizeof(signed char));
    if (!new_arr) return; /* out of memory */

    /* Linear sieve for μ */
    new_arr[1] = 1;
    idx_t *primes = (idx_t *)malloc(size * sizeof(idx_t));
    idx_t primes_cnt = 0;
    idx_t *is_comp = (idx_t *)calloc(size, sizeof(idx_t));

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            new_arr[i] = -1;
        }
        for (idx_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            idx_t m = i * p;
            if (m > n) break;
            is_comp[m] = 1;
            if (i % p == 0) {
                new_arr[m] = 0;
                break;
            } else {
                new_arr[m] = -new_arr[i];
            }
        }
    }

    free(primes);
    free(is_comp);

    free(mu_arr);
    mu_arr = new_arr;
    mu_max = n;
}

/* Compute a(n) based on the recurrence.  Works for any 64‑bit or arbitrary‑precision seq_t. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    ensure_mu(n);

    seq_t res = 1;          /* a(1) */
    if (n == 1) return res;

    for (idx_t k = 2; k <= n; ++k)
        res = (seq_t)k * res + (seq_t)mu_arr[k];

    return res;
}
