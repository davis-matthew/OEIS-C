/*
 * A176487  Triangle read by rows: T(n,k) = binomial(n,k) + A008292(n+1,k+1) -
 *          1.
 * https://oeis.org/A176487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r, k) + Eulerian(r+1, k) - 1, where r =
 *            floor((sqrt(8n+1)-1)/2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 95, u64 n <= 238,
 *            u128 n <= 608, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle is read by rows; for each term index n we find the
   corresponding row r and column k, compute binomial(r,k) and
   Eulerian(r+1,k) (Eulerian numbers via recurrence), then return
   binomial + Eulerian - 1. */
#include <stdlib.h>
#include "../utils/binom.h"

/* Space‑saving Eulerian numbers cache, indexed by row:
   euler_cache[n][k] stores Eulerian(n, k). */
static seq_t **euler_cache = NULL;
static idx_t euler_cache_rows = 0;

/* Ensure the cache contains rows 0 .. target_row (inclusive). */
static void ensure_eulerian_up_to(idx_t target_row)
{
    if (euler_cache_rows > target_row)
        return;

    seq_t **new_tbl = (seq_t **)realloc(euler_cache,
                                        sizeof(seq_t *) * (target_row + 1));
    if (!new_tbl)
        abort();                      /* Out of memory */

    euler_cache = new_tbl;

    for (idx_t n = euler_cache_rows; n <= target_row; ++n) {
        /* Allocate space for row n: n+1 entries (0..n). */
        euler_cache[n] = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!euler_cache[n])
            abort();

        if (n == 0) {
            /* A(0,0) = 1. */
            euler_cache[0][0] = 1;
        } else {
            seq_t *prev = euler_cache[n - 1];
            seq_t *curr = euler_cache[n];

            /* A(n,0) = A(n-1,0). */
            curr[0] = prev[0];

            /* For 1 <= k < n, use recurrence:
                    A(n,k) = (n-k)*A(n-1,k-1) + (k+1)*A(n-1,k). */
            for (idx_t k = 1; k < n; ++k) {
                curr[k] = (seq_t)((n - k) * prev[k - 1] + (k + 1) * prev[k]);
            }

            /* A(n,n) = 0 for n > 0. */
            curr[n] = 0;
        }
    }
    euler_cache_rows = target_row + 1;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column k within the row. */
    idx_t k = n - (idx_t)((unsigned __int128)r * (r + 1) / 2);

    /* We need Eulerian(r+1, k). */
    ensure_eulerian_up_to(r + 1);

    /* Compute the term using the definition. */
    seq_t bin = binom(r, k);
    seq_t eul = euler_cache[r + 1][k];
    seq_t result = bin + eul - 1;      /* All arithmetic is in seq_t. */
    return result;
}
