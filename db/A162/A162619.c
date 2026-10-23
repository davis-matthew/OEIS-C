/*
 * A162619  Triangle read by rows in which row n lists n consecutive natural
 *          numbers A000027, starting with A014689(n) = A000040(n)-n.
 * https://oeis.org/A162619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = prime(r) - r + (n - T(r-1) - 1) * where r is the smallest
 *            integer with T(r) = r(r+1)/2 >= n, * and T(k) = k(k+1)/2
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for known primes (1‑based) */
static uint64_t *prime_table   = NULL;   /* array of primes */
static size_t  prime_cnt      = 0;       /* number of primes stored   */
static size_t  prime_capacity = 0;       /* allocated entries         */

/* helper: grow the prime table to accommodate another prime */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_capacity) {
        prime_capacity = prime_capacity ? prime_capacity * 2 : 16;
        uint64_t *tmp = (uint64_t*)realloc(prime_table,
                           prime_capacity * sizeof(uint64_t));
        if (!tmp) exit(1);                 /* out of memory */
        prime_table = tmp;
    }
    prime_table[prime_cnt++] = p;
}

/* simple primality test using already known primes.
   Acceptable for all r needed for n up to the problem limits. */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_table[i];
        unsigned __int128 pp = (unsigned __int128)p * p;
        if (pp > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ensure that at least k primes are stored (1‑based indexing). */
static void ensure_primes(size_t k)
{
    while (prime_cnt < k) {
        uint64_t candidate;
        if (prime_cnt == 0) {
            candidate = 2;
            add_prime(candidate);
            continue;
        }
        candidate = prime_table[prime_cnt-1] + 1;
        if ((candidate & 1) == 0) candidate++;  /* skip even numbers */

        while (true) {
            if (is_prime_candidate(candidate)) {
                add_prime(candidate);
                break;
            }
            candidate += 2;  /* next odd candidate */
        }
    }
}

/* return the k‑th prime (k>=1). */
static seq_t get_prime(idx_t k)
{
    if (k == 0) return 0; /* unused, just for type safety */
    ensure_primes((size_t)k);
    return (seq_t)prime_table[k-1];
}

/* compute triangular number T(k) = k(k+1)/2 as seq_t. */
static seq_t triangular(idx_t k)
{
    __uint128_t t = (__uint128_t)k * (k + 1) / 2;
    return (seq_t)t;
}

/* find the row r such that T(r-1) < n <= T(r). */
static idx_t find_row(idx_t n)
{
    idx_t low  = 1;
    idx_t high = n;                 /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but keep defined */

    idx_t r   = find_row(n);                 /* row number (1‑based) */
    seq_t tpre= triangular(r-1);             /* T(r-1)           */
    idx_t pos = n - (idx_t)tpre - 1;         /* offset inside row */

    seq_t start = get_prime(r) - (seq_t)r;   /* A014689(r) */
    return start + (seq_t)pos;
}
