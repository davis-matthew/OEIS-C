/*
 * A080772  Numbers of numbers <= n having prime factors <= k, triangular
 *          array read by rows.
 * https://oeis.org/A080772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a( (r-1)r/2 + c ) = T(r,c) with T(r,r)=r and for 1<=c<r,
 *            T(r,c)=c if c+1 is prime, otherwise T(r,c)=T(r,c+1)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* primes array is lazily built up to the needed row index */
static bool *is_prime = NULL;
static size_t prime_limit = 0;
static bool prime_initialized = false;

/* build a sieve up to `lim`.  If a larger limit is later needed,
   it will be rebuilt. */
static void ensure_prime_limit(size_t lim)
{
    if (prime_initialized && prime_limit >= lim)
        return;

    free(is_prime);
    prime_limit = lim;
    is_prime = (bool *)malloc((prime_limit + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= prime_limit; ++i) is_prime[i] = true;
    if (prime_limit >= 0) is_prime[0] = false;
    if (prime_limit >= 1) is_prime[1] = false;
    for (size_t i = 2; i * i <= prime_limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= prime_limit; j += i)
                is_prime[j] = false;
        }
    }
    prime_initialized = true;
}

/* return the row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n */
static idx_t row_of(idx_t n)
{
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* behaviour for n < 1 is irrelevant */

    idx_t row = row_of(n);
    idx_t before = (row - 1) * row / 2;   /* number of terms before this row */
    idx_t col = n - before;               /* 1‑based column within the row */

    /* ensure primes are known up to this row index */
    ensure_prime_limit((size_t)row);

    if (col == row) return (seq_t)row;

    /* find the first prime p >= col+1 and <= row */
    for (idx_t p = col + 1; p <= row; ++p) {
        if (is_prime[p])
            return (seq_t)(p - 1); /* T(row, col) = p-1 */
    }
    return (seq_t)row; /* no such prime; propagate the rightmost value */
}
