/*
 * A204118  Symmetric matrix based on f(i,j) = gcd(prime(i), prime(j)), by
 *          antidiagonals.
 * https://oeis.org/A204118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n‑th element of the antidiagonal read of the matrix M
 *            with M[i][j] = gcd(prime_i, prime_j). That is, a(n) = prime_k if
 *            the n‑th element lies on the diagonal (i==j) and 1 otherwise
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* --- Prime generation (trial division) ---------------------------------- */
/* Store the primes we have computed so far (1‑based indexing). */
static uint32_t *prime_table = NULL;   /* prime_table[0] = 2, etc. */
static size_t    prime_count = 0;      /* number of primes stored       */
static size_t    prime_capacity = 0;   /* allocated capacity            */

/* Make sure the table holds primes up to at least `need` indices. */
static void
ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;

    /* grow capacity if necessary (power of two) */
    if (prime_capacity == 0) {
        prime_capacity = 64;
        prime_table = (uint32_t *)malloc(prime_capacity * sizeof(uint32_t));
    }
    while (prime_capacity < need) {
        prime_capacity <<= 1;
        prime_table = (uint32_t *)realloc(prime_table, prime_capacity * sizeof(uint32_t));
    }

    /* start from last known prime+1 */
    uint64_t cand = prime_count ? (uint64_t)prime_table[prime_count - 1] + 1u : 2u;

    while (prime_count < need) {
        /* Skip even numbers > 2 */
        if (cand == 2u || (cand & 1u)) {
            int is_prime = 1;
            /* trial division up to sqrt(cand) using integer arithmetic */
            for (size_t k = 0; k < prime_count; ++k) {
                uint64_t p = prime_table[k];
                if (p * p > cand)
                    break;
                if (cand % p == 0u) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                prime_table[prime_count++] = (uint32_t)cand;
                /* Ensure enough capacity after appending */
                if (prime_count == prime_capacity) {
                    prime_capacity <<= 1;
                    prime_table = (uint32_t *)realloc(prime_table, prime_capacity * sizeof(uint32_t));
                }
            }
        }
        ++cand;
    }
}

/* --- Find the diagonal that contains index n -------------------------------- */
static idx_t
find_diagonal(idx_t n)
{
    /* We need the smallest d with d*(d+1)/2 >= n  (d starts from 1). */
    idx_t lo = 1, hi = 1;
    while (1) {
        __uint128_t prod = ( __uint128_t )hi * ( hi + 1 );
        __uint128_t tri  = prod / 2;
        if (tri >= n)
            break;
        hi <<= 1;           /* double until we overshoot */
    }
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t prod = ( __uint128_t )mid * ( mid + 1 );
        __uint128_t tri  = prod / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* --- Main sequence function ---------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)   /* sequence is 1‑based; return 0 for undefined 0th term */
        return 0;

    /* Locate the diagonal d and the position k within that diagonal. */
    idx_t d = find_diagonal(n);                 /* diagonal length, 1‑based */
    idx_t prev_tri = (d - 1) * d / 2;           /* terms before diagonal d */
    idx_t k = n - prev_tri;                     /* 1‑based offset within diag */

    /* Corresponding matrix coordinates: i = k,   j = d + 1 - k   */
    idx_t i = k;
    idx_t j = d + 1 - k;

    /* Ensure we have computed enough primes. */
    size_t need = i > j ? i : j;
    ensure_primes(need);

    if (i == j) {
        /* Diagonal element – return the prime at index i (1‑based). */
        return (seq_t)prime_table[i - 1];
    } else {
        /* Off‑diagonal element – gcd of distinct primes is 1. */
        return (seq_t)1;
    }
}
