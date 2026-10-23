/*
 * A109024  Numbers that have exactly four prime factors counted with
 *          multiplicity (A014613) whose digit reversal is different and also
 *          has 4 prime factors (with multiplicity).
 * https://oeis.org/A109024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2609, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers that have exactly four prime factors (with multiplicity)
 * whose decimal reversal is different and also has exactly four prime factors. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Utility to convert an integer to seq_t, works in both C and C++ */
#ifdef __cplusplus
#define CAST_SEQ_T(x) static_cast<seq_t>(x)
#else
#define CAST_SEQ_T(x) ((seq_t)(x))
#endif

/* ------------------------------------------------------------------ */
/* Prime table ------------------------------------------------------- */
/* ------------------------------------------------------------------ */
static uint64_t *prime_table = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;
static uint64_t last_checked_prime = 1;   /* next candidate to test */

/* Ensure that all primes up to at least 'limit' are generated. */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= last_checked_prime) return;

    if (prime_cap == 0) {
        prime_cap = 64;
        prime_table = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_table) exit(EXIT_FAILURE);
    }

    while (last_checked_prime < limit) {
        ++last_checked_prime;
        if (last_checked_prime < 2) continue;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_table[i];
            if (p > last_checked_prime / p) break;
            if (last_checked_prime % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_table = (uint64_t*)realloc(prime_table, prime_cap * sizeof(uint64_t));
                if (!prime_table) exit(EXIT_FAILURE);
            }
            prime_table[prime_cnt++] = last_checked_prime;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Integer square‑root for uint64_t --------------------------------- */
/* ------------------------------------------------------------------ */
static uint64_t int_sqrt_uint64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        uint64_t nxt = r + bit;
        if (nxt <= x) {
            x -= nxt;
            r = nxt + bit;
        }
        bit >>= 2;
    }
    return r;
}

/* ------------------------------------------------------------------ */
/* Count prime factors (with multiplicity) of n (stop early >4). ----*/
/* ------------------------------------------------------------------ */
static int omega_uint64(uint64_t n)
{
    int cnt = 0;
    if (n < 2) return 0;
    uint64_t limit = int_sqrt_uint64(n) + 1;
    ensure_primes_upto(limit);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_table[i];
        if (p * p > n) break;
        while (n % p == 0) {
            ++cnt;
            n /= p;
            if (cnt > 4) return cnt;
        }
    }
    if (n > 1) ++cnt;
    return cnt;
}

/* ------------------------------------------------------------------ */
/* Reverse decimal digits of a 64‑bit integer. --------------------*/
/* ------------------------------------------------------------------ */
static uint64_t rev_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* ------------------------------------------------------------------ */
/* Term storage -----------------------------------------------------*/
/* ------------------------------------------------------------------ */
static seq_t *terms = NULL;
static size_t terms_size = 0;
static size_t terms_cap   = 0;

static void ensure_terms_cap(size_t min_cap)
{
    if (min_cap <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;

    seq_t *tmp;
#ifdef __cplusplus
    tmp = new seq_t[new_cap];
#else
    tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    if (!tmp) exit(EXIT_FAILURE);

    /* copy existing terms */
    for (size_t i = 0; i < terms_size; ++i) tmp[i] = terms[i];

#ifdef __cplusplus
    delete [] terms;
#else
    free(terms);
#endif
    terms = tmp;
    terms_cap = new_cap;
}

static void add_term(seq_t val)
{
    ensure_terms_cap(terms_size + 1);
    terms[terms_size++] = val;
}

/* ------------------------------------------------------------------ */
/* Main sequence function ---------------------------------------------*/
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static uint64_t cursor = 1;      /* last number examined          */
    static size_t found = 0;         /* number of terms stored so far */

    if (n == 0) return (seq_t)0;     /* out of bounds – undefined     */

    while (found < n) {
        ++cursor;
        if (omega_uint64(cursor) != 4) continue;

        uint64_t rev = rev_uint64(cursor);
        if (rev == cursor) continue;               /* skip palindromes */
        if (omega_uint64(rev) == 4) {
            add_term(CAST_SEQ_T(cursor));
            ++found;
        }
    }
    return terms[n - 1];
}
