/*
 * A232463  Number of ways to write n = p + q - pi(q), where p and q are odd
 *          primes not exceeding n, and pi(q) is the number of primes not
 *          exceeding q.
 * https://oeis.org/A232463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n = p + q - pi(q), where p and q
 *            are odd primes not exceeding n, and pi(q) is the number of
 *            primes <= q
 * Range:     u8 n <= 945, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* static state for primes */
static bool *is_prime = NULL;
static uint32_t *pi_arr = NULL;
static idx_t current_limit = 0;

/* extend prime tables up to at least n */
static void extend_primes(idx_t n)
{
    if (n <= current_limit) return;

    /* free old tables if they exist */
    if (is_prime) free(is_prime);
    if (pi_arr)   free(pi_arr);

    /* allocate new tables */
    is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    pi_arr   = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));

    /* initialise sieve */
    for (idx_t i = 0; i <= n; ++i) is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) is_prime[j] = false;
        }
    }

    /* build pi array */
    pi_arr[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        pi_arr[i] = pi_arr[i - 1] + (is_prime[i] ? 1 : 0);
    }

    current_limit = n;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* ensure tables cover n */
    extend_primes(n);

    seq_t count = 0;

    if (n < 3) return count;          /* no odd primes ≤ 2 */

    /* iterate over odd prime q */
    for (idx_t q = 3; q <= n; q += 2) {
        if (!is_prime[q]) continue;   /* skip non‑primes */

        uint32_t pi_q = pi_arr[q];
        /* compute p = n - q + pi(q) */
        idx_t p_val = (idx_t)((seq_t)n - (seq_t)q + (seq_t)pi_q);

        if (p_val > n) continue;       /* must be ≤ n */
        if (!is_prime[p_val]) continue;/* p must be prime */
        if (p_val == 2) continue;     /* only odd primes allowed */

        count += 1;
    }
    return count;
}
