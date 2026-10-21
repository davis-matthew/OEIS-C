/*
 * A085598  Primes p with same final decimal digit as k, p = prime(k).
 * https://oeis.org/A085598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A085598 – the n-th prime p for which the last decimal digit of p is
    equal to the last decimal digit of k, where k is the index of p in
    the prime sequence (p = prime(k)).  Off‑set: a(1) = 17.
    The routine generates primes and tests the condition until the
    required index is reached.  It keeps a cache of previously found
    primes and sequence values so that calls in arbitrary order are
    efficient.  The array holding the sequence values contains `uint64_t`
    entries only; they are cast to `seq_t` when returned.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------- cache for the sequence values --------------------------------- */
static uint64_t *seq_terms = NULL;     /* array of found primes */
static idx_t    seq_len  = 0;          /* number of sequence values stored */
static idx_t    seq_cap  = 0;          /* allocated capacity */

/* -------- cache for primes --------------------------------------------- */
static uint64_t *prime_vals      = NULL;   /* list of primes */
static size_t    prim_len        = 0;      /* number of primes known   */
static size_t    prim_cap        = 0;      /* allocated capacity of prime_vals */
static uint64_t  prime_candidate = 2;      /* next integer to test for primality */

/* -------- next index k that must be examined ---------------------------- */
static idx_t next_k = 1;

/* -------- enlarge seq_terms array -------------------------------------- */
static void ensure_seq_terms_cap(idx_t needed)
{
    if (needed <= seq_cap) return;
    idx_t new_cap = seq_cap > 0 ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(seq_terms, new_cap * sizeof(uint64_t));
    seq_terms = tmp;
    seq_cap   = new_cap;
}

/* -------- enlarge prime_vals array ------------------------------------- */
static void ensure_prime_vals_cap(size_t needed)
{
    if (needed <= prim_cap) return;
    size_t new_cap = prim_cap > 0 ? prim_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_vals, new_cap * sizeof(uint64_t));
    prime_vals = tmp;
    prim_cap   = new_cap;
}

/* -------- trial‑division primality test -------------------------------- */
static bool is_candidate_prime(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prim_len; ++i)
    {
        uint64_t p = prime_vals[i];
        if (p > n / p) break;           /* use division to avoid overflow */
        if (n % p == 0) return false;
    }
    return true;
}

/* -------- return the k‑th prime (1‑based) -------------------------------- */
static uint64_t get_prime(idx_t k)
{
    while (prim_len < (size_t)k)
    {
        while (!is_candidate_prime(prime_candidate))
            ++prime_candidate;
        ensure_prime_vals_cap(prim_len + 1);
        prime_vals[prim_len++] = prime_candidate;
        ++prime_candidate;
    }
    return prime_vals[k - 1];
}

/* -------- expand the sequence up to index n --------------------------------*/
static void generate_terms_up_to(idx_t n)
{
    if (n <= seq_len) return;
    ensure_seq_terms_cap(n);
    while (seq_len < n)
    {
        uint64_t p = get_prime(next_k);          /* p = prime(next_k)   */
        uint8_t  last_p = (uint8_t)(p % 10);
        uint8_t  last_k = (uint8_t)(next_k % 10);
        if (last_p == last_k)
            seq_terms[seq_len++] = p;
        ++next_k;
    }
}

/* -------- public entry point --------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* behaviour below offset is irrelevant */
    generate_terms_up_to(n);
    return (seq_t)seq_terms[n - 1];
}
