/*
 * A062720  If n is odd then 2*n else prime(n).
 * https://oeis.org/A062720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n if n is odd * a(n) = prime(n) if n is even * where
 *            prime(n) denotes the nth prime (1‑based, 2, 3, 5, 7, …)
 * Range:     u8 n <= 55, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

/* Ensure that at least 'need' primes are stored in prime_list.
   The list grows only when necessary and is populated by simple
   trial division. */
static void
ensure_prime_count(idx_t need)
{
    if (prime_cnt >= need) return;

    if (prime_cnt == 0) {
        if (!prime_list) {
            prime_cap = 8;
            prime_list = (idx_t *)malloc(sizeof(idx_t) * prime_cap);
            if (!prime_list) exit(1);
        }
        prime_list[prime_cnt++] = 2;
    }

    /* The next odd candidate after the last prime we already know. */
    idx_t candidate = (prime_cnt == 1) ? 3 : prime_list[prime_cnt - 1] + 2;

    while (prime_cnt < need) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap * 2;
                idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
                if (!tmp) exit(1);
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_cnt++] = candidate;
        }
        candidate += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined, but harmless */

    if (n & 1)                               /* odd n */
        return (seq_t)n * 2;
    else {                                   /* even n – nth prime */
        ensure_prime_count(n);
        return (seq_t)prime_list[n - 1];      /* zero‑based index */
    }
}
