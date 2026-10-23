/*
 * A165974  Irregular triangle read by rows: floor(j^2 / p) for p prime and j
 *          = 1 to p-1.
 * https://oeis.org/A165974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 6541, u16 n <= 8213, u32 n <= 8213, u64 n <= 8213,
 *            u128 n <= 8213, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: locate the prime row containing index n and
 * return floor(j^2 / p), where j is the column offset in that row. */
#include <stdlib.h>
#include <stdbool.h>

/* static data kept between calls */
static idx_t *primes = NULL;           /* array of discovered primes       */
static size_t primes_cnt   = 0;        /* number of primes stored          */
static size_t primes_cap   = 0;        /* allocated capacity of primes     */
static idx_t  current_candidate = 2;   /* next number to test for primality*/
static idx_t  total_terms  = 0;        /* total terms accumulated so far   */

/* add the next prime to the primes array, updating total_terms */
static void add_next_prime(void)
{
    for (;;) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt &&
                            primes[i] * primes[i] <= current_candidate; ++i) {
            if (current_candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* grow the array if needed */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap == 0 ? 16 : primes_cap * 2;
                idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                if (!tmp) abort();          /* allocation failed */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = current_candidate;
            total_terms += current_candidate - 1;   /* each prime adds p-1 terms */
            ++current_candidate;                   /* next candidate to test */
            return;
        }
        ++current_candidate; /* test next number */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1, return 0 */

    /* generate primes until the requested index is covered */
    while (total_terms < n) add_next_prime();

    /* locate the prime row that contains n */
    idx_t cum_before = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        idx_t row_len = p - 1;
        if (cum_before + row_len >= n) {
            idx_t j = n - cum_before;          /* 1‑based column index */
            return ((seq_t)j * (seq_t)j) / (seq_t)p;
        }
        cum_before += row_len;
    }

    /* this point should never be reached */
    return 0;
}
