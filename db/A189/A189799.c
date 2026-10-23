/*
 * A189799  Least position k of n (n written in base 2) in the infinite string
 *          101110111110111101100011001110111... (i.e., 23571113171923... in
 *          base 2).
 * https://oeis.org/A189799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least position k (1‑indexed) where the binary
 *            representation of n occurs in the infinite string obtained by
 *            concatenating the binary representations of all primes:
 *            2,3,5,7,11,13,17,19,23,... (i.e.,
 *            101110111110111101100011001110111…)
 * Range:     u8 n <= 41, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* --- Global state ---------------------------------------------------- */
static bool initialized = false;

static uint32_t *prime_list = NULL;
static size_t prime_count = 0, prime_cap = 0;
static uint32_t next_candidate = 3;   /* next odd candidate after 2 */

static char *bit_buffer = NULL;
static size_t bit_len = 0, bit_cap = 0;

static idx_t *cache_keys = NULL;
static seq_t *cache_vals = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* --- Helper prototypes ----------------------------------------------- */
static void init_globals(void);
static void ensure_prime_capacity(void);
static void ensure_bit_capacity(size_t need);
static void ensure_cache_capacity(void);
static uint32_t next_prime(void);
static void append_prime_bits(uint32_t prime);
static size_t find_substring(const char *needle, size_t nlen);
static void idx_to_bin(idx_t n, char *buf, size_t *len);

/* --- Helper implementations ------------------------------------------- */
static void init_globals(void)
{
    /* Prime list */
    prime_cap = 16;
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    prime_list[0] = 2;
    prime_count = 1;

    /* Bit buffer for the concatenated string */
    bit_cap = 2048;
    bit_buffer = (char *)malloc(bit_cap);
    /* Append binary of 2: "10" */
    bit_buffer[0] = '1';
    bit_buffer[1] = '0';
    bit_len = 2;

    /* Cache for computed a(n) values */
    cache_cap = 16;
    cache_keys = (idx_t *)malloc(cache_cap * sizeof(idx_t));
    cache_vals = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_len = 0;

    initialized = true;
}

static void ensure_prime_capacity(void)
{
    if (prime_count >= prime_cap) {
        prime_cap <<= 1;
        prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
    }
}

static void ensure_bit_capacity(size_t need)
{
    if (bit_len + need > bit_cap) {
        while (bit_len + need > bit_cap) bit_cap <<= 1;
        bit_buffer = (char *)realloc(bit_buffer, bit_cap);
    }
}

static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        cache_cap <<= 1;
        cache_keys = (idx_t *)realloc(cache_keys, cache_cap * sizeof(idx_t));
        cache_vals = (seq_t *)realloc(cache_vals, cache_cap * sizeof(seq_t));
    }
}

static uint32_t next_prime(void)
{
    while (1) {
        uint32_t cand = next_candidate;
        bool is_prime = true;
        uint32_t limit = (uint32_t) sqrt((double) cand);
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if (p > limit) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            ensure_prime_capacity();
            prime_list[prime_count++] = cand;
            next_candidate = cand + 2;          /* skip even numbers */
            return cand;
        }
        next_candidate += 2;
    }
}

static void append_prime_bits(uint32_t prime)
{
    size_t bitlen = 0;
    uint32_t tmp = prime;
    while (tmp) { ++bitlen; tmp >>= 1; }
    ensure_bit_capacity(bitlen);
    for (size_t i = 0; i < bitlen; ++i) {
        bit_buffer[bit_len + i] = ((prime >> (bitlen - 1 - i)) & 1) ? '1' : '0';
    }
    bit_len += bitlen;
}

static size_t find_substring(const char *needle, size_t nlen)
{
    if (nlen == 0 || bit_len < nlen) return SIZE_MAX;
    size_t end = bit_len - nlen;
    for (size_t i = 0; i <= end; ++i) {
        if (memcmp(bit_buffer + i, needle, nlen) == 0) return i;
    }
    return SIZE_MAX;
}

static void idx_to_bin(idx_t n, char *buf, size_t *len)
{
    if (n == 0) { buf[0] = '0'; *len = 1; return; }
    size_t bitlen = 0;
    idx_t t = n;
    while (t) { ++bitlen; t >>= 1; }
    for (size_t i = 0; i < bitlen; ++i) {
        buf[bitlen - 1 - i] = ((n >> i) & 1) ? '1' : '0';
    }
    *len = bitlen;
}

/* --- Main function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init_globals();

    /* Check cache */
    for (size_t i = 0; i < cache_len; ++i) {
        if (cache_keys[i] == n) return cache_vals[i];
    }

    /* Convert n to binary string */
    char pat[65];
    size_t plen;
    idx_to_bin(n, pat, &plen);

    /* Search until pattern appears */
    while (1) {
        size_t pos = find_substring(pat, plen);
        if (pos != SIZE_MAX) {
            seq_t ans = (seq_t)(pos + 1);    /* 1‑indexed */
            ensure_cache_capacity();
            cache_keys[cache_len] = n;
            cache_vals[cache_len] = ans;
            ++cache_len;
            return ans;
        }
        /* Append next prime's bits and try again */
        uint32_t p = next_prime();
        append_prime_bits(p);
    }
}
