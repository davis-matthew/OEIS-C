/*
 * A122845  Triangle read by rows, 3<=k<=n: T(n,k) = smallest prime p such
 *          that 2*k-p and 2*n-p are prime, T(n,k) = 0 if no such p exists.
 * https://oeis.org/A122845
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) gives the triangle of the sequence A122845 read by rows.
 *            T(n,k) is the smallest prime p such that 2k–p and 2n–p are
 *            prime, and a(n) enumerates T(n,k) with rows n=3,4,5,… and
 *            columns k=3,…,n
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Precompute small primes up to a limit sufficient for all rows involved
   (up to n ≈ 10000 corresponds to row < 200). */
#define PRIME_LIMIT 20000
static uint32_t primes[PRIME_LIMIT];
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;
    size_t i, j;
    bool *sieve = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    for (i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT)
                for (j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = true;
        }
    }
    free(sieve);
}

/* Compute T(row,k): smallest prime p such that 2*k-p and 2*row-p are prime.
   Return 0 if none exists. */
static seq_t triangle_entry(idx_t row, idx_t k)
{
    init_primes();
    idx_t two_k  = 2 * k;
    idx_t two_r  = 2 * row;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p > two_k) break;
        idx_t a = two_k - p;
        idx_t b = two_r - p;
        if (a >= 2 && b >= 2 &&
            is_prime_small((seq_t)a) &&
            is_prime_small((seq_t)b))
            return (seq_t)p;
    }
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3. For smaller indices, return 0. */
    if (n < 3) return 0;

    /* Find the row r containing the nth element (1‑based) in the triangle. */
    idx_t pos = n - 3;          /* zero‑based position within the entire list */
    idx_t r = 3;
    /* count(r) = sum_{i=3}^{r} (i-2) = (r-2)(r-1)/2 */
    while ((r - 2) * (r - 1) / 2 <= pos)
        ++r;

    /* Compute the column k within that row. */
    idx_t prev_count = (r - 3) * (r - 2) / 2;
    idx_t offset_in_row = pos - prev_count;
    idx_t k = 3 + offset_in_row;

    /* Return the triangle entry. */
    return triangle_entry(r, k);
}
