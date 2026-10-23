/*
 * A024479  a(n) = s(1)s(n) + s(2)s(n-1) + ... + s(k)s(n+1-k), where k = [
 *          (n+1)/2 ], s = (1, p(1), p(2), ...).
 * https://oeis.org/A024479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{⌊(n+1)/2⌋} s(i)*s(n+1-i), where s = (1, p(1),
 *            p(2), …) and p(k) is the k‑th prime
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime storage and generation ---------- */
static uint32_t *primes   = NULL;   /* primes[0] = 2 */
static size_t   primes_n  = 0;      /* number of stored primes */
static size_t   primes_cap = 0;     /* allocated capacity */

/* Grow the primes array to hold at least 'need' elements. */
static void grow_primes(size_t need)
{
    if (primes_cap >= need) return;
    size_t new_cap = primes_cap ? primes_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) abort();
    primes   = tmp;
    primes_cap = new_cap;
}

/* Test whether candidate is prime using the primes already stored. */
static inline bool candidate_is_prime(uint64_t cand)
{
    for (size_t i = 0; i < primes_n; ++i) {
        uint64_t p = primes[i];
        if (p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* Ensure that at least 'need' primes are computed. */
static void ensure_primes(size_t need)
{
    if (primes_n >= need) return;

    /* Initialise first prime if needed. */
    if (primes_n == 0) {
        grow_primes(1);
        primes[0] = 2;
        primes_n = 1;
    }

    /* Static variable keeps track of the next candidate to test. */
    static uint64_t cand = 3;

    while (primes_n < need) {
        if (cand == 3 || (cand & 1u)) { /* test odd candidates only */
            if (candidate_is_prime(cand)) {
                grow_primes(primes_n + 1);
                primes[primes_n++] = (uint32_t)cand;
            }
            cand += (cand == 3) ? 2 : 2; /* next odd number */
        } else {
            cand += 2; /* skip even numbers (only 2 is handled above) */
        }
    }
}

/* ---------- Auxiliary sequence s storage ---------- */
static seq_t *s_vals = NULL;    /* s_vals[1] = 1, s_vals[2] = p(1)=2, … */
static size_t s_len  = 0;        /* number of elements stored (≥1) */
static size_t s_cap  = 0;        /* allocated capacity */

/* Helper to allocate an array of seq_t that works in C and C++. */
#ifdef __cplusplus
#  define ALLOC_SEQ(n)  new seq_t[(n)]
#  define FREE_SEQ(p)   delete [] (p)
#else
#  define ALLOC_SEQ(n)  ((seq_t*)malloc((n)*sizeof(seq_t)))
#  define FREE_SEQ(p)   free(p)
#endif

/* Grow the s array to hold at least 'need' elements. */
static void grow_s_vals(size_t need)
{
    if (s_cap >= need) return;
    size_t new_cap = s_cap ? s_cap << 1 : 32;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = ALLOC_SEQ(new_cap);
    if (!tmp) abort();
    if (s_vals) {
        /* copy existing values; first element (index 0) is garbage */
        for (size_t i = 0; i < s_len; ++i) tmp[i] = s_vals[i];
        FREE_SEQ(s_vals);
    }
    s_vals = tmp;
    s_cap  = new_cap;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Ensure s_vals covers indices up to n. (n ≥ 1). */
    if (s_len < (size_t)n + 1) {
        grow_s_vals((size_t)n + 1);
        ensure_primes((size_t)n > 0 ? (size_t)n - 1 : 0);
        s_vals[1] = (seq_t)1;
        for (size_t i = 2; i <= (size_t)n; ++i) {
            s_vals[i] = (seq_t)primes[i - 2];  /* p(i-1) */
        }
        s_len = (size_t)n + 1;
    }

    seq_t result = (seq_t)0;
    idx_t k = (n + 1) / 2;            /* ⌊(n+1)/2⌋ */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t left  = s_vals[(size_t)i];
        seq_t right = s_vals[(size_t)(n + 1 - i)];
        result += left * right;
    }
    return result;
}
