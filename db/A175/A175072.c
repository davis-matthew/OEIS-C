/*
 * A175072  Natural numbers m with result 2 under iterations of {r mod (max
 *          prime p < r)} starting at r = m.
 * https://oeis.org/A175072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A175072 – Natural numbers m for which iterating r = r mod (max prime < r)
 * eventually reaches 2.  The function builds the sequence lazily by testing
 * successive integers in increasing order and caching the ones that satisfy
 * the property.  The cache is grown as needed; the computation is O(n log n)
 * and fast enough for n up to 10000. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Prime infrastructure -------------------------------------- */
static uint64_t *primes = NULL;          /* sorted list of primes found      */
static size_t   prime_cnt = 0;           /* number of primes stored          */
static size_t   prime_cap = 0;           /* allocated capacity for primes    */
static uint64_t last_prime_checked = 1;  /* largest number verified for primality */

/* Ensure that all primes <= limit are in the 'primes' array.  */
static void ensure_primes_upto (uint64_t limit)
{
    if (limit <= last_prime_checked)
        return;

    for (uint64_t cand = last_prime_checked + 1; cand <= limit; ++cand) {
        /* Skip even numbers greater than 2 – all other composites are
           rejected by trial division below. */
        if (cand > 2 && (cand & 1) == 0)
            continue;

        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p)        /* avoid overflow, stop once p > sqrt(cand) */
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp) abort();  /* allocation failure */
                primes = tmp;
            }
            primes[prime_cnt++] = cand;
        }
    }
    last_prime_checked = limit;
}

/* Return the largest prime strictly less than x, or 0 if none. */
static uint64_t largest_prime_lt (uint64_t x)
{
    if (x <= 2)
        return 0;
    /* We need all primes up to just below x. */
    ensure_primes_upto(x - 1);

    size_t lo = 0, hi = prime_cnt;  /* hi is exclusive */
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0)
        return 0;
    return primes[lo - 1];
}

/* ---------- Cache for the sequence ----------------------------------- */
static seq_t *seq_cache = NULL;   /* array of computed terms */
static size_t seq_cap   = 0;      /* allocated capacity for cache */
static idx_t  seq_len   = 0;      /* number of terms currently stored */
static uint64_t next_candidate = 2; /* next integer to test */

/* Append a new term to the cache, growing it if necessary. */
static void add_value (seq_t v)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
    }
    seq_cache[seq_len++] = v;
}

/* Test whether m eventually reaches 2 under r = r mod (max prime < r). */
static bool converges_to_2 (seq_t m)
{
    if (m == (seq_t)2)
        return true;
    if (m == (seq_t)1)
        return false;

    while (m > (seq_t)2 && m > (seq_t)1) {
        uint64_t r = (uint64_t)m;
        uint64_t p = largest_prime_lt(r);
        if (p == 0)    /* should not happen for r > 2 */
            return false;
        r %= p;
        m = (seq_t)r;
    }
    return (m == (seq_t)2);
}

/* --------------------------------------------------------------------- */
seq_t a (idx_t n)
{
    /* Lazy initialization of fundamental structures */
    if (seq_len == 0) {
        seq_cap = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) abort();
        /* primes array is empty; primes discovered on demand. */
    }

    /* Generate terms until we reach the n-th.  */
    while (seq_len < n) {
        seq_t candidate = (seq_t)next_candidate;
        if (converges_to_2(candidate))
            add_value(candidate);
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
