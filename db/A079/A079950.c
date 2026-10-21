/*
 * A079950  Triangle of n-th prime modulo twice primes less n-th prime.
 * https://oeis.org/A079950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A079950 – Triangle of n-th prime modulo twice primes < n-th prime.
    For a positive integer n, let (r,c) be the row and column of the
    n-th entry in the row‑major enumeration of the triangle

           T(r,c) = prime(r) mod (2*prime(c))

    where rows and columns are indexed starting at 1.  Then
    a(n) = T(r,c).  The routine below computes this value for any n ≥ 1.
    Primes are generated on demand by a simple incremental trial‑division
    algorithm.  The implementation uses only integer arithmetic and
    works for all supported integer sizes.  */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *prime_cache = NULL;   /* array of generated primes   */
static size_t   prime_cnt   = 0;       /* number of primes stored    */
static size_t   prime_alloc = 0;       /* allocated capacity         */
static uint64_t next_candidate = 3;    /* next odd number to test   */

/* Generate primes until prime_cnt >= needed.  */
static void ensure_primes(size_t needed)
{
    if (prime_cnt >= needed) return;

    if (prime_cnt == 0) {          /* start with 2 */
        if (prime_alloc < 1) {
            prime_alloc = 8;
            prime_cache = (uint64_t *)realloc(prime_cache, prime_alloc * sizeof(uint64_t));
        }
        prime_cache[prime_cnt++] = 2;
    }

    /* increase storage if required */
    if (prime_alloc < prime_cnt + needed - prime_cnt) {
        size_t new_alloc = prime_alloc * 2;
        if (new_alloc < prime_cnt + (needed - prime_cnt)) new_alloc = prime_cnt + (needed - prime_cnt);
        prime_cache = (uint64_t *)realloc(prime_cache, new_alloc * sizeof(uint64_t));
        prime_alloc = new_alloc;
    }

    while (prime_cnt < needed) {
        uint64_t cand = next_candidate;
        int is_prime = 1;
        /* trial division by all stored primes up to sqrt(cand) */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            prime_cache[prime_cnt++] = cand;
        }
        next_candidate += 2;          /* next odd candidate */
    }
}

/* Return the (row,column) indices for the n-th element in a row‑major
   traversal of the triangle.  Returns row and col via parameters.  */
static void row_col_from_index(uint64_t n, uint64_t *row, uint64_t *col)
{
    /* Find r such that r(r+1)/2 >= n > (r-1)r/2 */
    uint64_t low = 1, high = n;          /* high is safe upper bound */
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n) high = mid;
        else low = mid + 1;
    }
    *row = low;
    __uint128_t prev_count = (__uint128_t)(low - 1) * low / 2;
    *col = n - (uint64_t)prev_count;
}

/* Main routine as required by the specification */
seq_t a(idx_t n)
{
    /* n is 1‑based by OEIS convention */
    uint64_t row, col;
    row_col_from_index(n, &row, &col);

    /* Ensure we have primes up to the required indices */
    ensure_primes((size_t)row);   /* row <= col <= row */

    uint64_t prime_r = prime_cache[row - 1];
    uint64_t prime_c = prime_cache[col - 1];

    __uint128_t mod_base = (__uint128_t)prime_c * 2;
    __uint128_t res128 = (__uint128_t)prime_r % mod_base;

    return (seq_t)res128;
}
