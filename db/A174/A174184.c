/*
 * A174184  Prime(n)+even nonprime(n) is prime.
 * https://oeis.org/A174184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A174184:
 * a(n) is the n‑th index i (offset 1) such that prime(i) + even_nonprime(i) is prime.
 * even_nonprime(i) = 0 for i = 1, otherwise 2·i.
 * The implementation builds the required primes on demand and
 * caches the qualifying indices for repeated calls. */
 
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --------------------------- Prime sieve --------------------------- */
static uint64_t *prime_list = NULL;    /* primes in increasing order */
static size_t   prime_cap   = 0;
static size_t   prime_cnt   = 0;
static size_t   sieve_limit = 0;

/* Grow the prime list array */
static void grow_prime_list(void)
{
    prime_cap = prime_cap ? prime_cap * 2 : 16;
    prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);
}

/* Sieve primes up to 'limit' and store them in prime_list */
static void extend_sieve_to(size_t limit)
{
    if (limit <= sieve_limit) return;
    /* Free old list and allocate a fresh one */
    free(prime_list);
    prime_list = NULL;

    uint8_t *isComp = (uint8_t*)calloc(limit + 1, 1);
    if (!isComp) exit(EXIT_FAILURE);

    double ln = log((double)limit);
    size_t est = (size_t)(limit / (ln > 0.0 ? ln : 1.0)) + 10;
    prime_cap = est;
    prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_list) { free(isComp); exit(EXIT_FAILURE); }

    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!isComp[i]) {
            if (prime_cnt == prime_cap) grow_prime_list();
            prime_list[prime_cnt++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    isComp[j] = 1;
            }
        }
    }
    free(isComp);
    sieve_limit = limit;
}

/* Return the k‑th prime (1‑based). */
static seq_t prime_at(idx_t k)
{
    if (k == 0) return 0;          /* unused */
    while (prime_cnt < (size_t)k) {
        size_t new_limit = sieve_limit ? sieve_limit * 2 : 32;
        extend_sieve_to(new_limit);
    }
    return (seq_t)prime_list[(size_t)k - 1];
}

/* ----------------- Qualifying indices storage ------------------- */
static idx_t *qual_idx = NULL;
static size_t qual_cap = 0;
static size_t qual_cnt = 0;
static idx_t  last_index = 0;

/* Append next qualifying index to the array */
static void append_qual(idx_t val)
{
    if (qual_cnt == qual_cap) {
        qual_cap = qual_cap ? qual_cap * 2 : 8;
        qual_idx = (idx_t*)realloc(qual_idx, qual_cap * sizeof(idx_t));
        if (!qual_idx) exit(EXIT_FAILURE);
    }
    qual_idx[qual_cnt++] = val;
}

/* --------------------------- Main function ----------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 only defined for n >= 1 */

    while (qual_cnt < (size_t)n) {
        ++last_index;                      /* candidate index i */

        seq_t p = prime_at(last_index);    /* prime(i) */
        seq_t e = (last_index == 1) ? 0 : (seq_t)2 * (seq_t)last_index; /* even_nonprime(i) */
        seq_t sum = p + e;

        if (is_prime_big(sum)) {
            append_qual(last_index);
        }
    }

    return (seq_t)qual_idx[n - 1];
}
