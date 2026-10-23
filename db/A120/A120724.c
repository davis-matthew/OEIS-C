/*
 * A120724  Irregular triangle T(n, k) = prime(n) + k*(k-1), for n >= 1 and 1
 *          <= k <= 1 + A000720(n), read by rows.
 * https://oeis.org/A120724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 222, u16 n <= 1565, u32 n <= 1565, u64 n <= 1565,
 *            u128 n <= 1565, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A120724 – Irregular triangle
    a(n) is the n‑th element of the triangle
        T(r,k) = prime(r) + k*(k-1),   1 ≤ k ≤ 1+π(r)
    where π(r) is the number of primes ≤ r.
    The sequence is read row by row.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Maximum value for the pre‑computed sieve.
   For n up to 10000 the required row index is < 1000,
   so 20000 is more than enough. */
#define MAX_SIEVE 20000

/* Static tables used by the function. */
static uint8_t  is_prime_arr[MAX_SIEVE + 1];
static uint64_t prime_list[MAX_SIEVE]   = {0};
static size_t   prime_cnt   = 0;
static size_t   pi_of[MAX_SIEVE + 1]   = {0};

/* Cumulative terms up to each row: cum_terms[0] = 0 (sentinel). */
static seq_t   cum_terms[MAX_SIEVE + 1];
static size_t  cum_terms_len = 0;

/* Number of rows that have been built (equal to cum_terms_len-1). */
static size_t  built_rows = 0;

/* One‑time sieve initialisation. */
static void init_sieve(void)
{
    /* initialise is_prime_arr: 0 and 1 are not prime */
    for (size_t i = 0; i <= MAX_SIEVE; ++i) {
        is_prime_arr[i] = 1;
    }
    is_prime_arr[0] = is_prime_arr[1] = 0;

    for (size_t i = 2; i <= MAX_SIEVE; ++i) {
        if (is_prime_arr[i]) {
            /* store prime */
            prime_list[prime_cnt++] = (uint64_t)i;
            /* sifting */
            for (uint64_t j = (uint64_t)i * i; j <= MAX_SIEVE; j += i) {
                is_prime_arr[j] = 0;
            }
        }
        pi_of[i] = pi_of[i - 1] + is_prime_arr[i];
    }
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0) {          /* offset is 1; a(0) is undefined – return 0 */
        return 0;
    }

    /* Initialise sieve the first time the function is called. */
    static bool sieve_done = false;
    if (!sieve_done) {
        init_sieve();
        /* initialise cumulative terms table with only the sentinel. */
        cum_terms[0] = 0;
        cum_terms_len = 1;
        built_rows = 0;
        sieve_done = true;
    }

    /* Build rows until we have enough terms. */
    while (cum_terms_len == 0 || cum_terms[cum_terms_len - 1] < n) {
        size_t next_row = built_rows + 1;          /* row numbers start at 1 */

        if (next_row > MAX_SIEVE) {               /* safety check */
            abort();
        }

        size_t row_len = 1 + pi_of[next_row];     /* 1 + π(r) columns */

        seq_t prev_cum = cum_terms_len == 0 ? 0 : cum_terms[cum_terms_len - 1];
        seq_t new_cum  = prev_cum + (seq_t)row_len;

        if (cum_terms_len > MAX_SIEVE) {          /* safety check */
            abort();
        }
        cum_terms[cum_terms_len] = new_cum;
        ++cum_terms_len;
        ++built_rows;
    }

    /* Locate the row containing the required term. */
    size_t lo = 1;
    size_t hi = built_rows;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cum_terms[mid] < n) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    size_t row = lo;

    /* Compute column index (k) within the row. */
    seq_t k = (seq_t)(n - cum_terms[row - 1]);   /* 1‑based column */

    /* Retrieve prime(row). */
    seq_t prime_row = (seq_t)prime_list[row - 1];

    /* Evaluate T(row, k) = prime(row) + k*(k-1). */
    seq_t kk = k;
    seq_t term = prime_row + kk * (kk - (seq_t)1);

    return term;
}
