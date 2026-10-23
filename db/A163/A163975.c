/*
 * A163975  n-th nonprime - (-1)^(n-th nonprime).
 * https://oeis.org/A163975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = nth_nonprime - (-1)^(nth_nonprime)
 * Range:     i8 n <= 98, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_LIMIT 200000UL

static bool ready = false;
static bool is_prime[MAX_LIMIT + 1];
static seq_t nonprimes[MAX_LIMIT + 1];
static idx_t nonprime_count = 0;

/* Build sieve and store non‑prime numbers up to MAX_LIMIT.  */
static void init_tables(void)
{
    idx_t i, p, j;

    for (i = 0; i <= MAX_LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (p = 2; p * p <= MAX_LIMIT; ++p) {
        if (is_prime[p]) {
            for (j = p * p; j <= MAX_LIMIT; j += p)
                is_prime[j] = false;
        }
    }

    nonprime_count = 0;
    for (i = 0; i <= MAX_LIMIT; ++i) {
        if (!is_prime[i])
            nonprimes[nonprime_count++] = (seq_t)i;
    }

    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready)
        init_tables();

    /* the sequence is 1‑indexed */
    idx_t idx = n - 1;
    /* safety for out‑of‑range n; only needed if n > nonprime_count */
    if (idx >= nonprime_count) {
        /* fallback: extend search linearly (unlikely for n ≤ 10000) */
        seq_t m = nonprimes[nonprime_count - 1];
        while (idx >= nonprime_count) {
            ++m;
            /* simple primality test for remaining numbers */
            bool prime = true;
            if (m < 2)
                prime = false;
            else {
                for (seq_t d = 2; d * d <= m; ++d) {
                    if (m % d == 0) {
                        prime = false;
                        break;
                    }
                }
            }
            if (!prime) {
                if (nonprime_count < MAX_LIMIT)
                    nonprimes[nonprime_count++] = m;
                if (idx == nonprime_count - 1)
                    break;
            }
        }
    }

    seq_t m = nonprimes[idx];
    seq_t sign = (m & 1) ? -1 : 1;
    return m - sign;
}
