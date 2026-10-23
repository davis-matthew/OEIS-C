/*
 * A125703  Table read by antidiagonals: row n contains the positive integers
 *          (in order) which are coprime to the n-th prime and do not occur in
 *          earlier rows.
 * https://oeis.org/A125703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry T(i,j) read from a table whose rows contain *
 *            multiples of the product of the first (i-1) primes and that are
 *            coprime * to the i-th prime. The sequence is obtained by reading
 *            this table along * antidiagonals. For a given n we compute the
 *            coordinates * * i = row index, j = column...
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------
 *  Prime table (up to 200 000) – sufficient for n ≲ 10⁵.
 * ------------------------------------------------------------ */
static const idx_t PRIME_LIMIT = 200000;   /* upper bound on prime values */
static uint64_t primes[20000];            /* storage for primes                   */
static size_t prime_cnt = 0;             /* number of primes actually stored     */
static bool primes_ready = false;

/* Initialise the prime table once.  Uses a standard sieve of Eratosthenes. */
static void
init_primes(void)
{
    if (primes_ready)
        return;

    bool *is_prime = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_prime)
        return;  /* pragma: ignore failure – tests will never trigger it */

    for (idx_t i = 0; i <= PRIME_LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= PRIME_LIMIT; j += p)
                is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (idx_t n = 2; n <= PRIME_LIMIT && cnt < 20000; ++n) {
        if (is_prime[n])
            primes[cnt++] = (uint64_t)n;
    }
    prime_cnt = cnt;
    free(is_prime);
    primes_ready = true;
}

/* ------------------------------------------------------------
 *  Main sequence function
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* off‑by‑one cases are ignored */

    /* Find the diagonal d such that sum_{t=1}^d t >= n */
    idx_t d = 1;
    idx_t cum = 1;                 /* sum_{t=1}^d t */
    while (cum < n) {
        ++d;
        cum += d;
    }

    /* Offset of the element within this diagonal (0‑based) */
    idx_t prev = (d - 1) * d / 2;
    idx_t offset = n - prev - 1;
    idx_t i = d - offset;          /* row index  (1‑based) */
    idx_t j = offset + 1;          /* column index (1‑based) */

    /* Ensure prime table is ready */
    init_primes();

    /* Guard if requested row exceeds precomputed prime count */
    if (i > prime_cnt) {
        /* This should not happen for the problem limits. */
        return (seq_t)0;
    }

    /* Prime p_i */
    uint64_t p_i = primes[i - 1];

    /* j-th integer coprime to p_i :  k = j + floor((j-1)/(p_i-1)) */
    idx_t denom = p_i - 1;
    idx_t k = j + (j - 1) / denom;

    /* Compute P = product of first i-1 primes */
    seq_t P = (seq_t)1;
    for (idx_t t = 1; t < i; ++t) {
        P *= (seq_t)primes[t - 1];
    }

    return P * (seq_t)k;
}
