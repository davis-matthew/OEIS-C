/*
 * A218017  Triangle, read by rows, where T(n,k) = k!*C(n, k)*7^(n-k) for
 *          n>=0, k=0..n.
 * https://oeis.org/A218017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = k! * C(r,k) * 7^(r-k), * where r and k are the
 *            antidiagonal coordinates of n: * r*(r+1)/2 ≤ n < (r+1)*(r+2)/2,
 *            * k = n - r*(r+1)/2. * * The function caches factorials and
 *            powers of 7 to compute the * term efficiently for any n (O(r)
 *            memoized growth)
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 67, u64 n <= 201, u128 n <= 582,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *fact_cache = NULL;      /* factorials: fact[i] = i!  (i ≥ 0) */
static seq_t *pow7_cache = NULL;      /* powers of 7: pow7[i] = 7^i */
static idx_t fact_len = 0;            /* current length of factorial cache */
static idx_t pow7_len = 0;            /* current length of power‑of‑7 cache */

/* Ensure factorial cache includes [0..n] */
static void ensure_fact(idx_t n)
{
    if (fact_len > n) return;
    size_t new_len = n + 1;
    fact_cache = (seq_t *)realloc(fact_cache, new_len * sizeof(seq_t));
    for (size_t i = fact_len; i < new_len; ++i) {
        fact_cache[i] = (seq_t)(i == 0 ? 1 : fact_cache[i - 1] * (seq_t)i);
    }
    fact_len = new_len;
}

/* Ensure 7^i cache includes [0..n] */
static void ensure_pow7(idx_t n)
{
    if (pow7_len > n) return;
    size_t new_len = n + 1;
    pow7_cache = (seq_t *)realloc(pow7_cache, new_len * sizeof(seq_t));
    for (size_t i = pow7_len; i < new_len; ++i) {
        pow7_cache[i] = (seq_t)(i == 0 ? 1 : pow7_cache[i - 1] * (seq_t)7);
    }
    pow7_len = new_len;
}

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Grow high until the triangular number exceeds n */
    while ((high * (high + 1ULL)) / 2 <= n) {
        high <<= 1;
    }
    /* Binary search */
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        if ((mid * (mid + 1ULL)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine coordinates (r,k) of n in the triangle */
    idx_t r = find_row(n);
    idx_t first_in_row = (r * (r + 1ULL)) / 2;
    idx_t k = n - first_in_row;

    /* Precompute factorials and powers of 7 up to r and (r-k) as needed */
    ensure_fact(r);
    ensure_pow7(r - k);

    /* T(r,k) = (r! / (r-k)!) * 7^(r-k) */
    seq_t ratio = fact_cache[r] / fact_cache[r - k];
    return ratio * pow7_cache[r - k];
}
