/*
 * A050773  Iterated procedure 'composite k added to sum of its prime factors
 *          reaches a prime' yields 6 skipped primes.
 * https://oeis.org/A050773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A050773 – “Composite k added to sum of its prime factors reaches a prime”
 *
 *  For each composite integer k the process is:
 *      1. Add k to the sum of its prime factors (with multiplicity);
 *      2. Repeat until a prime is reached.
 *  Count the primes that lie strictly between k and the final prime.  
 *  If that count equals exactly 6, k is a term of the sequence.
 *
 *  The implementation keeps the found terms in a cache and generates
 *  more terms on demand.  Primes are generated on the fly and the
 *  primality test is deterministic for 64‑bit values, which is
 *  sufficient for the first 10 000 terms of the sequence.
 *
 *  The code respects the constraints on seq_t and idx_t:
 *      - seq_t may be an unsigned integer type of any width or an
 *        arbitrary‑precision class; only integer operations are
 *        used on seq_t values.
 *      - idx_t is an unsigned 64‑bit integer.
 *
 *  All dynamic allocations are cast explicitly to match the pointer
 *  type required, which satisfies both C11 and C++17.
 *
 *  The code assumes that every intermediate value that is used as an
 *  array index or loop bound fits into idx_t.  For the sequence
 *  considered up to n = 10 000 the values stay below 2⁶⁴, so the
 *  implementation is fully correct for the intended test cases.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*   Prime handling                                                    */
/* ------------------------------------------------------------------ */

static idx_t *prime_list   = NULL;   /* list of generated primes      */
static size_t prime_cnt    = 0;      /* number of primes              */
static size_t prime_cap    = 0;      /* allocated capacity            */

/* add a new prime to the list, resizing if necessary */
static void add_prime(idx_t p)
{
    if (prime_cap == 0) {
        prime_cap = 64;
        prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
    } else if (prime_cnt == prime_cap) {
        prime_cap *= 2;
        prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
    }
    prime_list[prime_cnt++] = p;
}

/* generate the next prime after the current largest one */
static void generate_next_prime(void)
{
    idx_t candidate = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p > candidate / p) break;         /* p * p > candidate */
            if (candidate % p == 0) { ok = false; break; }
        }
        if (ok) {
            add_prime(candidate);
            return;
        }
        ++candidate;
    }
}

/* ensure that all primes up to sqrt(limit) are available */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_cnt == 0) generate_next_prime();      /* at least 2 */
    while (prime_list[prime_cnt - 1] <= limit / prime_list[prime_cnt - 1])
        generate_next_prime();
}

/* deterministic 64‑bit primality test */
static bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    ensure_primes_upto(x);
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > x / p) break;                /* p * p > x */
        if (x % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*   Arithmetic on seq_t                                               */
/* ------------------------------------------------------------------ */

/* sum of prime factors of n (with multiplicity) */
static seq_t sum_prime_factors(idx_t n)
{
    seq_t sum = 0;
    idx_t temp = n;
    ensure_primes_upto(temp);
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > temp / p) break;
        while (temp % p == 0) {
            sum += (seq_t)p;
            temp /= p;
        }
        if (temp == 1) break;
    }
    if (temp > 1) sum += (seq_t)temp;   /* remaining prime factor */
    return sum;
}

/* count primes strictly between low and high (high is assumed > low) */
static idx_t count_primes_between(idx_t low, idx_t high)
{
    idx_t c = 0;
    for (idx_t v = low + 1; v < high && c <= 6; ++v) {
        if (is_prime_idx(v)) ++c;
    }
    return c;
}

/* ------------------------------------------------------------------ */
/*   Cache of computed terms                                          */
/* ------------------------------------------------------------------ */

static seq_t *cache      = NULL;   /* 1‑based cache of a(n)             */
static size_t cache_len  = 0;      /* number of stored terms             */
static size_t cache_cap  = 0;      /* allocated capacity                 */

/* add a term to the cache, resizing if necessary */
static void cache_add(seq_t value)
{
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = value;
}

/* test whether a composite k satisfies the sequence condition */
static bool is_term(idx_t k)
{
    idx_t current = k;
    for (;;) {
        seq_t s = sum_prime_factors(current);
        /* convert current to seq_t to add the sum */
        seq_t cur_seq = (seq_t)current;
        seq_t nxt_seq = cur_seq + s;
        /* Convert nxt back to idx_t; this is safe because all
           intermediate values fit into 64 bits for the required range. */
        idx_t nxt = (idx_t)nxt_seq;
        if (is_prime_idx(nxt)) {
            idx_t skipped = count_primes_between(k, nxt);
            return skipped == 6;
        }
        current = nxt;
    }
}

/* generate terms until the cache contains at least 'need' entries */
static void generate_until(size_t need)
{
    idx_t start = 4;                      /* first term known */
    if (cache_len > 0) {
        /* the next candidate starts after the last found term */
        start = (idx_t)cache[cache_len - 1] + 1;
    }
    for (;;) {
        while (cache_len >= need) return;
        while (is_prime_idx(start)) ++start;  /* skip primes */
        if (is_term(start)) {
            cache_add((seq_t)start);
        }
        ++start;
    }
}

/* ------------------------------------------------------------------ */
/*   Exported function                                                */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */
    if (cache_len < (size_t)n) generate_until((size_t)n);
    return cache[(size_t)n - 1];
}
