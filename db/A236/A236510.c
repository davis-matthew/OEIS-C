/*
 * A236510  Numbers whose prime factorization viewed as a tuple of powers is
 *          palindromic, when viewed from the least to the largest prime
 *          present, including also any zero-exponents for the intermediate
 *          primes.
 * https://oeis.org/A236510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A236510: numbers whose prime factorization exponent tuple
 * (with zeros for omitted intermediate primes, but excluding leading
 * and trailing zeros) is a palindrome.  The sequence is generated
 * on demand by testing each integer in increasing order and keeping
 * a cache of the found terms.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* ---------- Prime generation utilities ---------- */
static uint64_t *prime_arr = NULL;   /* array of primes */
static size_t   prime_cnt = 0;      /* number of primes in array */
static uint64_t prime_limit = 0;    /* current upper bound of generated primes */

/* Build a list of all primes <= limit, freeing any old list. */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit) return;
    if (prime_cnt > 0) free(prime_arr);
    build_primes_upto(limit, &prime_arr, &prime_cnt);
    prime_limit = limit;
}

/* Return index of prime p in prime_arr (0‑based).  p must be a prime. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_arr[mid] == p) return mid;
        if (prime_arr[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return prime_cnt;               /* should not happen for a prime */
}

/* ---------- Exponent‑palindrome test ---------- */
static bool is_exponent_palindrome(uint64_t n)
{
    if (n == 1) return true;                /* empty tuple is a palindrome */

    /* Factor n; we assume n fits in uint64_t. */
    uint64_t fac_pr[64];
    uint64_t fac_ex[64];
    size_t   fac_cnt = 0;
    factor_u64(n, fac_pr, fac_ex, &fac_cnt);
    if (fac_cnt == 0) return true;          /* just in case */

    /* The factor routine returns primes in ascending order. */
    uint64_t pmax = fac_pr[fac_cnt - 1];
    ensure_primes_upto(pmax);                /* guarantee we have indices */

    size_t min_idx = SIZE_MAX, max_idx = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        size_t idx = prime_index(fac_pr[i]);
        if (idx < min_idx) min_idx = idx;
        if (idx > max_idx) max_idx = idx;
    }

    size_t sz = max_idx + 1 - min_idx;
    if (sz == 0) return true;                /* should not happen */

    int *exps = (int *)calloc(sz, sizeof(int));
    if (!exps) return false;                 /* trust undef as non‑palindrome */

    for (size_t i = 0; i < fac_cnt; ++i) {
        size_t idx = prime_index(fac_pr[i]) - min_idx;
        exps[idx] = (int)fac_ex[i];
    }

    bool ok = true;
    for (size_t i = 0, j = sz - 1; i < j; ++i, --j) {
        if (exps[i] != exps[j]) { ok = false; break; }
    }
    free(exps);
    return ok;
}

/* ---------- Sequence cache ---------- */
static seq_t *seq_vals   = NULL;   /* array of sequence values */
static size_t seq_cap    = 0;      /* capacity of seq_vals */
static size_t seq_len    = 0;      /* number of stored terms */
static uint64_t next_candidate = 1;/* next integer to consider */

/* Append a value to the sequence cache. */
static void add_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) return; /* out of memory – terminate silently */
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* ---------- The sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined but harmless */

    /* Return from cache if already computed. */
    if ((size_t)n <= seq_len) return seq_vals[(size_t)n - 1];

    /* Generate terms until we reach the desired index. */
    while (seq_len < (size_t)n) {
        if (is_exponent_palindrome(next_candidate))
            add_seq((seq_t)next_candidate);
        ++next_candidate;
    }
    return seq_vals[(size_t)n - 1];
}
