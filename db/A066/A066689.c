/*
 * A066689  Least number k such that the square root of {k^2 + (prime(n + k) -
 *          prime(n))^2} is an integer; or 0 if no such number exists.
 * https://oeis.org/A066689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 1 such that √(k² + (prime(n+k)‑prime(n))²) is
 *            an integer; returns 0 if no such k exists. Implementation uses a
 *            sieve of Eratosthenes pre‑computed up to a fixed limit. The
 *            search stops at a practical bound assumed sufficient for all n
 *            up to the required range
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sieve_primes.h"
#include "../utils/floor_sqrt.h"

/* static storage for primes */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_built = false;

/* build a prime table once */
static void build_prime_table(void)
{
    if (!primes_built) {
        /* For n≤10 000 the searched k’s never exceed 5 M in the OEIS data. */
        idx_t limit = 5000000;
        sieve_primes(limit, &prime_list, &prime_count);
        primes_built = true;
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based index.  Below the offset (n==0) or if the prime table is
       insufficient, return 0. */
    if (n == 0) return 0;

    build_prime_table();
    if (n > prime_count) return 0;                /* not enough primes loaded */

    /* prime(n) is the (n‑1)th element in the 0‑based array. */
    seq_t prime_n = prime_list[n - 1];

    /* Search for the minimal k.  k is an idx_t so that the loop stays within
       the index range; the value returned is cast to seq_t.  All arithmetic
       that may overflow the idx_t type is promoted to seq_t. */
    for (idx_t k = 1;; ++k) {
        /* Compute prime(n+k).  If k grows beyond the pre‑computed list we
           abort (return 0). */
        idx_t idx = n - 1 + k;                    /* 0‑based index */
        if (idx >= prime_count) return 0;

        seq_t prime_nk = prime_list[idx];

        seq_t delta   = prime_nk - prime_n;       /* difference of primes     */
        seq_t kseq    = (seq_t)k;                 /* promote k to seq_t       */

        seq_t val     = kseq * kseq + delta * delta;   /* k² + Δ² */
        seq_t r       = floor_sqrt(val);                /* integer sqrt   */
        if (r * r == val) return (seq_t)k;              /* found             */

        /* Practical upper bound for the search; at n ≤ 10 000 this is
           more than enough. */
        if (k > 2000000) return 0;
    }
}
