/*
 * A143791  A positive integer k is included if no prime divisor p of k, when
 *          p is represented in binary, occurs within k represented in binary.
 * https://oeis.org/A143791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 6733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9334 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A143791.  It is defined by a search:
   a(1)=1 and for n>1, a(n) is the n-th odd composite k such that
   for every prime divisor p of k the binary representation of p
   does not occur as a substring of the binary representation of k. */

/* ---------- include dependencies ---------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* ---------- binary conversion ---------- */
static size_t bin_str(seq_t x, char *buf)
{
    size_t pos = 0;
    seq_t tmp = x;
    while (tmp != 0)
    {
        buf[pos++] = (tmp & 1) ? '1' : '0';
        tmp >>= 1;
    }
    /* reverse into correct order */
    for (size_t i = 0; i < pos / 2; ++i)
    {
        char t = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = t;
    }
    return pos;
}

/* ---------- substring check ---------- */
static bool exists_substring(const char *big, size_t blen,
                             const char *pat, size_t plen)
{
    if (plen == 0 || plen > blen) return false;
    for (size_t i = 0; i + plen <= blen; ++i)
        if (memcmp(big + i, pat, plen) == 0)
            return true;
    return false;
}

/* ---------- helper tables ---------- */
static const uint64_t PRIME_LIMIT   = 100000;           /* enough for 10k terms       */
static uint64_t        primes[100000];
static size_t          primes_cnt = 0;
static unsigned char   is_comp[100001] = {0};

/* ---------- initialise primes ---------- */
static void init_primes(void)
{
    if (primes_cnt != 0) return;          /* already initialised       */
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
    {
        if (!is_comp[i])
        {
            primes[primes_cnt++] = i;
            if (i * i <= PRIME_LIMIT)
                for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
        }
    }
}

/* ---------- test if a number is in the sequence ---------- */
static bool in_sequence(seq_t k)
{
    if (k == 1) return true;                     /* the only unit in the list */
    if ((k & 1) == 0) return false;              /* even numbers are excluded */

    /* binary string of k */
    char k_bits[130];
    size_t k_len = bin_str(k, k_bits);

    seq_t rem = k;

    /* iterate over primes up to sqrt(rem) */
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        if (p * p > rem) break;
        if (rem % p == 0)
        {
            /* prime factor p found – test substring */
            char p_bits[130];
            size_t p_len = bin_str(p, p_bits);
            if (exists_substring(k_bits, k_len, p_bits, p_len))
                return false;
            /* divide out all occurrences of p */
            while (rem % p == 0) rem /= p;
        }
    }
    /* remaining factor larger than sqrt(k) */
    if (rem > 1)
    {
        char p_bits[130];
        size_t p_len = bin_str(rem, p_bits);
        if (exists_substring(k_bits, k_len, p_bits, p_len))
            return false;
    }
    return true;
}

/* ---------- cache of computed terms ---------- */
static seq_t *cache          = NULL;
static size_t cache_cap      = 0;
static size_t cache_len      = 0;
static idx_t  next_candidate = 1;

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    /* initialise global data the first time */
    if (cache_len == 0)
    {
        init_primes();                          /* primes for factorisation   */

        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
    }

    /* grow cache if needed */
    while (cache_len < (size_t)n)
    {
        seq_t val = next_candidate;

        if (in_sequence(val))
        {
            if (cache_len == cache_cap)
            {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = val;
        }

        /* advance candidate – skip evens after 1 */
        if (next_candidate == 1) next_candidate = 3;
        else                      next_candidate += 2;
    }

    return cache[n - 1];
}
