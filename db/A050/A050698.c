/*
 * A050698  Numbers that appear in A050695 but not in A035139.
 * https://oeis.org/A050698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   num_to_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050698:
   a(n) is the nth positive integer whose prime divisors
   satisfy:
     * No prime divisor is a decimal substring of the integer.
     * At least one digit of the prime divisors appears in the integer.
   The function generates terms by scanning integers from 2 upward
   and testing the above conditions.  No formula is known. */

/* Since the build may use an arbitrary‑precision class for seq_t,
   all arithmetic is done via the provided seq_t type.  This code
   is valid both as C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/num_to_digits.h"

/* upper bound for the sieve of small primes */
#define SPF_MAX 10000000U

/*---------------------------------------------*/
/*  Helpers for decimal conversion and test   */

static bool contains_substring(const char *big, int biglen,
                               const char *small, int smalllen)
{
    if (smalllen > biglen) return false;
    for (int i = 0; i <= biglen - smalllen; ++i) {
        if (big[i] == small[0]) {
            bool ok = true;
            for (int j = 1; j < smalllen; ++j) {
                if (big[i + j] != small[j]) { ok = false; break; }
            }
            if (ok) return true;
        }
    }
    return false;
}

/*---------------------------------------------*/
/*  Linear sieve for small primes             */
/*---------------------------------------------*/
static uint32_t *spf = NULL;          /* smallest prime factor for each integer up to SPF_MAX */
static uint32_t *prime_list = NULL;   /* array of primes up to SPF_MAX */
static size_t    prime_cnt  = 0;
static size_t    prime_cap  = 0;

static void init_sieve(void)
{
    if (spf != NULL) return;          /* already initialised */

    spf = (uint32_t*)calloc((size_t)SPF_MAX + 1U, sizeof(uint32_t));
    if (!spf) abort();

    /* temporary array for primes during construction */
    size_t  tmp_cap = SPF_MAX / 10U + 1U;
    uint32_t *tmp_pr = (uint32_t*)malloc(tmp_cap * sizeof(uint32_t));
    if (!tmp_pr) abort();

    prime_cnt = 0;
    prime_cap = tmp_cap;

    spf[0] = 0;
    spf[1] = 1;

    for (uint32_t i = 2; i <= SPF_MAX; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                uint32_t *newpr = (uint32_t*)realloc(tmp_pr, prime_cap * sizeof(uint32_t));
                if (!newpr) abort();
                tmp_pr = newpr;
            }
            tmp_pr[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt && tmp_pr[j] <= spf[i] &&
             (uint64_t)i * tmp_pr[j] <= SPF_MAX; ++j) {
            spf[i * tmp_pr[j]] = tmp_pr[j];
        }
    }
    prime_list = tmp_pr;  /* keep pointer for later usage */
}

/*---------------------------------------------*/
/*  Validation for numbers <= SPF_MAX          */
/*---------------------------------------------*/
static bool is_valid_small(uint32_t num)
{
    char m_buf[20], p_buf[20];
    int m_len, p_len, i;
    uint16_t m_mask = 0;
    uint32_t temp = num;
    bool digit_found = false;

    num_to_digits(num, m_buf, &m_len);
    for (i = 0; i < m_len; ++i) m_mask |= 1u << (m_buf[i] - '0');

    while (temp > 1) {
        uint32_t p = spf[temp];
        while (temp % p == 0) temp /= p;

        num_to_digits(p, p_buf, &p_len);
        if (contains_substring(m_buf, m_len, p_buf, p_len)) return false;

        for (i = 0; i < p_len; ++i) {
            int d = p_buf[i] - '0';
            if (m_mask & (1u << d)) { digit_found = true; break; }
        }
    }
    return digit_found;
}

/*---------------------------------------------*/
/*  Validation for numbers > SPF_MAX           */
/*---------------------------------------------*/
static bool is_valid_large(uint64_t num)
{
    char m_buf[20], p_buf[20];
    int m_len, p_len, i;
    uint16_t m_mask = 0;
    uint64_t temp = num;
    bool digit_found = false;

    num_to_digits(num, m_buf, &m_len);
    for (i = 0; i < m_len; ++i) m_mask |= 1u << (m_buf[i] - '0');

    for (size_t idx = 0; idx < prime_cnt && (uint64_t)prime_list[idx] * prime_list[idx] <= temp; ++idx) {
        uint32_t p = prime_list[idx];
        if (temp % p != 0) continue;
        while (temp % p == 0) temp /= p;

        num_to_digits(p, p_buf, &p_len);
        if (contains_substring(m_buf, m_len, p_buf, p_len)) return false;

        for (i = 0; i < p_len; ++i) {
            int d = p_buf[i] - '0';
            if (m_mask & (1u << d)) { digit_found = true; break; }
        }
    }
    if (temp > 1) {                     /* remaining large prime factor */
        uint64_t p = temp;
        num_to_digits(p, p_buf, &p_len);
        if (contains_substring(m_buf, m_len, p_buf, p_len)) return false;
        for (i = 0; i < p_len; ++i) {
            int d = p_buf[i] - '0';
            if (m_mask & (1u << d)) { digit_found = true; break; }
        }
    }
    return digit_found;
}

/*---------------------------------------------*/
/*  Cache and main routine for a(idx_t n)      */
/*---------------------------------------------*/
static seq_t *cache      = NULL;
static size_t cache_sz   = 0;
static size_t cache_cap  = 0;
static uint64_t next_scan = 2;   /* next integer to test */

/* cache helper */
static void cache_push(seq_t val)
{
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = val;
}

/* the function required by the build */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;   /* undefined for non‑positive n */

    size_t idx_needed = (size_t)n;
    init_sieve();                  /* make sure sieve is ready */

    if (cache_sz >= idx_needed)
        return cache[idx_needed - 1];

    /* keep scanning until we have enough terms */
    while (cache_sz < idx_needed) {
        bool ok;
        if (next_scan <= SPF_MAX)
            ok = is_valid_small((uint32_t)next_scan);
        else
            ok = is_valid_large(next_scan);
        if (ok) cache_push((seq_t)next_scan);
        ++next_scan;
    }
    return cache[idx_needed - 1];
}
