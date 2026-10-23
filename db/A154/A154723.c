/*
 * A154723  Triangle read by rows in which row n lists all the pairs of
 *          noncomposite numbers that are equidistant from n, or only n if
 *          there are no such pairs, as shown below in the example.
 * https://oeis.org/A154723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1802, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A154723: a(n) is the n-th element of the triangle read by
 * rows, where each row r lists all pairs of noncomposite numbers
 * equidistant from r (sorted by the smaller number then the larger).
 * If no such pair exists, the row contains only r.
 * The function generates rows until it reaches the required
 * n-th entry, using a sieve-based prime table for primality tests.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* ---------- Prime table (lazy initialization) ---------- */
    static bool prime_tbl_init = false;
    static bool *is_prime = NULL;   /* prime flag for 0..prime_limit */
    static idx_t *primes = NULL;    /* list of primes (excluding 1) */
    static size_t prime_cnt = 0;
    const idx_t prime_limit = 400000; /* enough for n <= 10000 rows */

    if (!prime_tbl_init) {
        /* Allocate and compute sieve */
        is_prime = (bool *)calloc((size_t)prime_limit + 1, sizeof(bool));
        if (!is_prime) exit(EXIT_FAILURE);
        for (idx_t i = 2; i <= prime_limit; ++i) is_prime[i] = true;
        for (idx_t p = 2; p * p <= prime_limit; ++p) {
            if (is_prime[p]) {
                for (idx_t j = p * p; j <= prime_limit; j += p)
                    is_prime[j] = false;
            }
        }
        /* Count primes */
        for (idx_t i = 2; i <= prime_limit; ++i)
            if (is_prime[i]) ++prime_cnt;
        primes = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
        if (!primes) exit(EXIT_FAILURE);
        size_t idx = 0;
        for (idx_t i = 2; i <= prime_limit; ++i)
            if (is_prime[i]) primes[idx++] = i;
        prime_tbl_init = true;
    }

    /* ---------- Find the row that contains the n-th element ---------- */
    idx_t row = 1;
    idx_t cumulative = 0; /* number of entries processed so far */

    /* Buffers for smaller and larger numbers in a row */
    idx_t smaller[2600];
    idx_t larger[2600];

    while (1) {
        size_t pair_cnt = 0;

        /* Check pair with x = 1 (if applicable) */
        if (row > 1) {
            idx_t y = 2 * row - 1;
            bool y_noncomposite = (y == 1) || (y <= prime_limit && is_prime[y]);
            if (y_noncomposite) {
                smaller[pair_cnt] = 1;
                larger[pair_cnt] = y;
                ++pair_cnt;
            }
        }

        /* Iterate over primes < row */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = primes[i];
            if (p >= row) break;          /* only primes smaller than row */
            idx_t x = p;
            idx_t d = row - x;            /* d > 0 */
            idx_t y = row + d;
            bool y_noncomposite = (y == 1) || (y <= prime_limit && is_prime[y]);
            if (y_noncomposite) {
                smaller[pair_cnt] = x;
                larger[pair_cnt] = y;
                ++pair_cnt;
            }
        }

        /* Compute length of this row */
        seq_t row_len = (pair_cnt == 0) ? 1 : 2 * (seq_t)pair_cnt;

        if (cumulative + (idx_t)row_len < n) {
            /* Row not reached; accumulate and move to next row */
            cumulative += (idx_t)row_len;
            ++row;
            continue;
        }

        /* The desired element lies in this row */
        idx_t pos_in_row = n - cumulative; /* 1‑based position */

        if (pair_cnt == 0) {
            /* Only one element: the row number itself */
            return (seq_t)row;
        }

        if (pos_in_row <= (idx_t)pair_cnt) {
            /* Element is in the smaller numbers list (already sorted ascending) */
            return (seq_t)smaller[pos_in_row - 1];
        } else {
            /* Element is in the larger numbers list.
             *  The 'larger' array was filled with y = 2*row - p,
             *  where p was in ascending order. Thus 'larger' is in
             *  descending order.  We need the (pos-in-row - pair_cnt)-th
             *  smallest, i.e., the index from the end. */
            size_t idx = pos_in_row - pair_cnt - 1;       /* 0‑based */
            return (seq_t)larger[pair_cnt - 1 - idx];
        }
    }
}
