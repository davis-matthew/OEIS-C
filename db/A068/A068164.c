/*
 * A068164  Smallest prime obtained from n by inserting zero or more decimal
 *          digits.
 * https://oeis.org/A068164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the smallest prime that contains the decimal digits of n
 *            as a subsequence (digits may be inserted before, after, or
 *            between digits of n)
 * Range:     u8 n <= 11, u16 n <= 1001, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/*  Standard C/C++ headers
   ------------------------------------------------------------ */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
/*  Utility functions
   ------------------------------------------------------------ */

/* Convert seq_t value to a decimal string stored in buf.
///   buf must have enough space (at least 50 bytes is safe for 128‑bit values). */
static void seq_to_str(seq_t x, char *buf)
{
    if (x == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    char tmp[64];
    int pos = 0;
    while (x != 0) {
        tmp[pos++] = '0' + (int)(x % 10);
        x /= 10;
    }
    for (int i = 0; i < pos; ++i) buf[i] = tmp[pos-1-i];
    buf[pos] = '\0';
}

/* Does the prime p contain the decimal string s of length slen as a subsequence? */
static bool contains_subseq(seq_t p, const char *s, int slen)
{
    char buf[128];
    seq_to_str(p, buf);
    int i = 0, j = 0;
    while (buf[i] && j < slen) {
        if (buf[i] == s[j]) ++j;
        ++i;
    }
    return j == slen;
}

/* Convert idx_t to a decimal string stored in buf (no leading zeros). */
static void idx_to_str(idx_t n, char *buf)
{
    if (n == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    char tmp[32];
    int pos = 0;
    while (n != 0) {
        tmp[pos++] = '0' + (int)(n % 10);
        n /= 10;
    }
    for (int i = 0; i < pos; ++i) buf[i] = tmp[pos-1-i];
    buf[pos] = '\0';
}

/* ------------------------------------------------------------ */
/*  Prime generation
   ------------------------------------------------------------ */

static seq_t *prime_list = NULL;       /* primes in ascending order */
static size_t   prime_cnt  = 0;         /* number of primes stored */
static idx_t    primes_limit = 0;       /* highest number sieved */

/* Rebuild the prime table so that it contains all primes up to limit */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= primes_limit) return;

    size_t sz = (size_t)limit + 1;
    bool *sieve = (bool *)calloc(sz, sizeof(bool));
    if (!sieve) return;   /* allocation failure – unlikely for the limits we use */

    for (size_t i = 2; i * (size_t)i <= limit; ++i)
        if (!sieve[i])
            for (size_t j = i * (size_t)i; j <= (size_t)limit; j += i)
                sieve[j] = true;

    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (!sieve[i]) ++cnt;

    seq_t *new_primes = (seq_t *)malloc(cnt * sizeof(seq_t));
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (!sieve[i]) new_primes[idx++] = (seq_t)i;

    free(prime_list);
    prime_list = new_primes;
    prime_cnt  = cnt;
    primes_limit = limit;
    free(sieve);
}

/* Binary search: first prime in prime_list that is >= val */
static size_t find_prime_ge(seq_t val)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    return lo;   /* may equal prime_cnt if not found */
}

/* ------------------------------------------------------------ */
/*  Sequence cache
   ------------------------------------------------------------ */

static seq_t *cache_vals = NULL; /* a(n) values */
static bool   *cache_ok = NULL;  /* whether cache_vals[n] is set */
static size_t  cache_cap = 0;    /* allocated size */

/* Ensure cache can hold index n */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 128;
    while ((size_t)n >= new_cap) new_cap <<= 1;
    cache_vals = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
    cache_ok   = (bool   *)realloc(cache_ok,   new_cap * sizeof(bool));
    memset(cache_ok + cache_cap, 0, (new_cap - cache_cap) * sizeof(bool));
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Main function
   ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined – offset starts at 1 */

    ensure_cache_size(n);
    if (cache_ok[n]) return cache_vals[n];

    /* Convert n to a decimal string
       -------------------------------- */
    char key[32];
    idx_to_str(n, key);
    int keylen = 0; while (key[keylen]) ++keylen;

    /* Initial prime limit: 20 million – enough for n≤10 000 */
    const idx_t INITIAL_PRIMES = 20000000UL;
    ensure_primes_up_to(INITIAL_PRIMES);

    seq_t n_seq = (seq_t)n;
    size_t start = find_prime_ge(n_seq);

    for (size_t i = start; i < prime_cnt; ++i) {
        seq_t p = prime_list[i];
        if (contains_subseq(p, key, keylen)) {
            cache_vals[n] = p;
            cache_ok[n]   = true;
            return p;
        }
    }

    /* If not found within the current prime table, extend the sieve and try again */
    idx_t limit = primes_limit;
    while (true) {
        limit *= 2;                     /* double the limit */
        ensure_primes_up_to(limit);     /* rebuild the prime table */
        start = find_prime_ge(n_seq);   /* restart from the first prime >= n */
        for (size_t i = start; i < prime_cnt; ++i) {
            seq_t p = prime_list[i];
            if (contains_subseq(p, key, keylen)) {
                cache_vals[n] = p;
                cache_ok[n]   = true;
                return p;
            }
        }
        if (limit > 1000000000UL) break;   /* safety guard – should never happen */
    }

    /* If we reach here something went wrong (theoretically impossible for the given bounds). */
    return 0;
}
