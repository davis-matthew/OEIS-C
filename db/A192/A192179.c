/*
 * A192179  Array determined by distance to next prime, by antidiagonals.
 * https://oeis.org/A192179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n-th entry of the table * T[r,c] = k where k is the
 *            smallest positive integer * with least h>0 such that k+h is
 *            prime (h=r). * The table is read along anti‑diagonals: * a(n) =
 *            T[r,c] where r = n − T_{d-1} and * d is the smallest integer
 *            with d(d+1)/2 ≥ n. * The table lists every positi...
 * Range:     u8 n <= 97, u16 n <= 1824, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Prime generation utilities – simple trial‑division sieve            */
/* ------------------------------------------------------------------ */
static idx_t *prime_tbl = NULL;      /* dynamic array of primes           */
static size_t prime_cnt = 0;          /* number of primes stored           */
static size_t prime_cap = 0;          /* current allocation capacity       */
static idx_t prime_last = 1;          /* largest number we have tested     */

/* Grow the prime array if needed */
static void grow_prime_array(void)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap * 2 + 1;
        idx_t *new_tbl = (idx_t *)realloc(prime_tbl, new_cap * sizeof(idx_t));
        if (!new_tbl) exit(EXIT_FAILURE);       /* out of memory */
        prime_tbl = new_tbl;
        prime_cap = new_cap;
    }
}

/* Is number x prime? Uses the primes already generated */
static bool is_prime_simple(idx_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_tbl[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure that all primes ≤ limit are generated and stored in prime_tbl */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_last) return;

    for (idx_t n = prime_last + 1; n <= limit; ++n) {
        if (is_prime_simple(n)) {
            grow_prime_array();
            prime_tbl[prime_cnt++] = n;
        }
    }
    prime_last = limit;
}

/* Binary search: first index i with prime_tbl[i] > value */
static size_t first_prime_gt(idx_t value)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_tbl[mid] <= value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/*  Primary function: a(idx_t n)                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for 0, return 0 */

    /* ---- Locate anti‑diagonal -------------------------------------- */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n) ++d;     /* smallest d with T_d ≥ n */

    idx_t tri_prev = ((d - 1) * d) / 2;     /* T_{d-1} */
    idx_t r = n - tri_prev;                /* row number (1‑based)  */
    idx_t c = d - r + 1;                    /* position in that row */

    /* ---- Find the c‑th element of row r --------------------------- */
    /* Ensure we have enough primes (rough estimate) */
    ensure_primes_upto(r + 2000);           /* primes larger than r needed */

    size_t start_idx = first_prime_gt(r);   /* first prime > r          */
    size_t idx = start_idx;
    size_t found = 0;

    while (true) {
        if (idx >= prime_cnt) {
            /* need more primes – double the limit */
            ensure_primes_upto(prime_last * 2 + 1000);
            /* after ensuring, recalculate start_idx if necessary */
            if (idx >= prime_cnt) return (seq_t)0; /* safety fall‑back */
        }
        idx_t p = prime_tbl[idx];

        idx_t prev_p = (idx > 0) ? prime_tbl[idx - 1] : 0;
        idx_t gap = p - prev_p;

        if (gap >= r) {
            ++found;
            if (found == c) {
                /* k = p - r */
                return (seq_t)(p - r);
            }
        }
        ++idx;
    }
}
