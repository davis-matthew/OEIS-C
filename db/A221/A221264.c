/*
 * A221264  Numbers having fewer distinct prime factors of form 4*k+1 than of
 *          4*k+3.
 * https://oeis.org/A221264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A221264 (numbers having fewer distinct
 * prime factors of the form 4*k+1 than of 4*k+3) by incremental
 * search.  The algorithm keeps a cache of already found terms so
 * that any call to a(idx_t n) is answered in O(1) after an
 * initial sweep up to that index. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Prime sieve for small primes --------------------------------- */
#define PRIME_LIMIT 2000
static uint64_t prime_list[PRIME_LIMIT];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    bool is_composite[PRIME_LIMIT] = { false };
    for (size_t i = 2; i < PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt < PRIME_LIMIT)
                prime_list[prime_cnt++] = i;
            for (size_t j = i * i; j < PRIME_LIMIT; j += i)
                is_composite[j] = true;
        }
    }
    primes_ready = true;
}

/* ---------- Test whether a number satisfies the OEIS condition ----- */
static bool is_good(uint64_t v)
{
    uint64_t n = v;
    unsigned cnt1 = 0, cnt3 = 0;          /* counts of 4k+1 and 4k+3 primes */
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            if (p % 4 == 1) ++cnt1;
            else if (p % 4 == 3) ++cnt3;
            do { n /= p; } while (n % p == 0);
        }
    }
    if (n > 1) {                         /* remaining prime factor */
        if (n % 4 == 1) ++cnt1;
        else if (n % 4 == 3) ++cnt3;
    }
    return cnt1 < cnt3;
}

/* ---------- Cache for memoised values --------------------------------- */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;
static idx_t  last_checked = 1;          /* last integer examined */

/* Ensure capacity for at least new_len entries */
static void ensure_cache_capacity(size_t new_len)
{
    if (new_len <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();             /* allocation failed */
    seq_cache = new_cache;
    seq_cap   = new_cap;
}

/* ---------- Main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 1, safe return */

    if (n <= seq_len) return seq_cache[n - 1];   /* already computed */

    init_primes();

    while (seq_len < (size_t)n) {
        ++last_checked;
        if (is_good((uint64_t)last_checked)) {
            ensure_cache_capacity(seq_len + 1);
            seq_cache[seq_len] = (seq_t)last_checked;
            ++seq_len;
        }
    }
    return seq_cache[n - 1];
}
