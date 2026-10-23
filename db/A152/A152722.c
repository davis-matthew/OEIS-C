/*
 * A152722  Triangle read by rows: T(n,0) = prime(n+2), T(n,1) = 1 - T(n,0),
 *          T(n,k) = T(n-1,k-1), T(1,0) = 1 T(n,n) = -1.
 * https://oeis.org/A152722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less, obsc
 * Formula:   a(n) is the n‑th element of the triangle read row‑wise. The
 *            triangle is defined by T(n,0) = prime(n+2) for n≥2, except
 *            T(1,0)=1, T(0,0)=-1 T(n,1) = 1 – T(n,0) T(n,k) = T(n-1,k-1) for
 *            k≥2 T(n,n) = -1 The sequence lists the rows in order: -1 ; 1, -1
 *            ; 7, -6, -1 ; 11, -10, -6, -1 ; …
 * Range:     i8 n <= 464, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Prime table: grow on demand
   --------------------------------------------------------------------- */
static uint32_t *prime_table = NULL;
static size_t     prime_cnt   = 0;
static size_t     prime_cap   = 0;

/* integer square root for 32‑bit unsigned */
static uint32_t isqrt_uint32(uint32_t x)
{
    uint32_t low = 0, high = 65536;   /* ceil(sqrt(2^32‑1)) */
    while (low < high) {
        uint32_t mid = (low + high + 1) >> 1;
        if ((uint64_t)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Ensure that prime_table contains at least k primes (1‑based) */
static void ensure_prime_k(idx_t k)
{
    if (prime_cnt >= (size_t)k)
        return;

    if (prime_cap == 0) {
        prime_cap = 128;
        prime_table = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    }

    if (prime_cnt == 0) {                 /* start with 2 */
        prime_table[prime_cnt++] = 2;
    }

    uint32_t cand = prime_table[prime_cnt - 1] + 1;

    while (prime_cnt < (size_t)k) {
        uint32_t lim  = isqrt_uint32(cand);
        bool     prime = true;
        for (size_t i = 0; i < prime_cnt && prime_table[i] <= lim; ++i) {
            if (cand % prime_table[i] == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_table = (uint32_t *)realloc(prime_table, prime_cap * sizeof(uint32_t));
            }
            prime_table[prime_cnt++] = cand;
        }
        ++cand;
    }
}

/* Return the k‑th prime (1‑based) as seq_t */
static seq_t get_prime_k(idx_t k)
{
    ensure_prime_k(k);
    return (seq_t)prime_table[(size_t)k - 1];
}

/* --------------------------------------------------------------------- */
/* Helper: find row index r for given global index n
   --------------------------------------------------------------------- */
static idx_t find_row(idx_t n)
{
    /* find smallest r such that (r+1)(r+2)/2 > n */
    idx_t low = 0, high = 1;
    /* increase high until cumulative count exceeds n */
    while (((__int128)(high + 1) * (high + 2)) / 2 <= n)
        high <<= 1;   /* double */

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 cum = ((__int128)(mid + 1) * (mid + 2)) / 2;
        if (cum <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

/* --------------------------------------------------------------------- */
/* Main sequence function
   --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Determine row r and column j within that row */
    idx_t r = find_row(n);
    /* cumulative terms up to row r-1 */
    __int128 before = (__int128)r * (r + 1) / 2;
    idx_t j = (idx_t)(n - (idx_t)before);

    /* Special case: last element of a row */
    if (j == r)
        return (seq_t)-1;

    /* Column 0 */
    if (j == 0) {
        if (r == 1)
            return (seq_t)1;                        /* special value */
        else
            return get_prime_k((idx_t)r + 2);        /* prime(r+2) */
    }

    /* Column 1 */
    if (j == 1) {
        seq_t base;
        if (r == 1)
            base = (seq_t)1;                       /* special */
        else
            base = get_prime_k((idx_t)r + 2);       /* prime(r+2) */
        return (seq_t)1 - base;
    }

    /* Columns j ≥ 2 */
    /* Recursively step j-1 times: new row = r - (j-1), column = 1 */
    idx_t new_r = r - (j - 1);
    seq_t base;
    if (new_r == 1)
        base = (seq_t)1;
    else
        base = get_prime_k((idx_t)new_r + 2);
    return (seq_t)1 - base;
}
