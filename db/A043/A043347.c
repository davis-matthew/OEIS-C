/*
 * A043347  To form the sequence, write the primes 2, 3, 5, 7,... as an
 *          infinite string 2357111317...; then take the first 2 digits of the
 *          string, 23, then the next 3 digits, 571, then the next 5 digits,
 *          11317, then the next 7 digits, 1923293, then... (stepping through
 *          prime numbers of digits). Omit any lead...
 * https://oeis.org/A043347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of prime digits, taken in blocks of lengths
 *            equal to the first n primes (2,3,5,7,11,…). Leading zeros in
 *            each block are omitted
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   digit_value, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../utils/next_prime.h"   /* next_prime(uint64_t start) -> next prime > start */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */
#include "../utils/digit_value.h"

/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*  a(idx_t n) - compute the n-th term of A043347, with caching             */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state retained between calls                                 */
    static uint64_t *prime_arr      = NULL;    /* primes generated so far            */
    static size_t   prime_cnt        = 0;        /* number of primes stored            */
    static size_t   prime_cap        = 0;        /* capacity of prime_arr              */

    static char  *digits_buf         = NULL;    /* concatenated digits of primes      */
    static size_t buf_len            = 0;        /* current length of digits_buf       */
    static size_t buf_cap            = 0;        /* capacity of digits_buf             */

    static size_t used_prime_idx     = 0;        /* primes whose digits are already     */
                                                /* appended to digits_buf              */
    static size_t curr_offset        = 0;        /* next unused digit position         */

    static seq_t *cache              = NULL;    /* computed a(k) values               */
    static size_t cache_cap          = 0;        /* number of slots in cache array     */
    static size_t cache_used         = 0;        /* high index that has been computed  */

    /* --------------------------------------------------------------------- */
    /*  Return cached value if already computed                               */
    /* --------------------------------------------------------------------- */
    if (n <= cache_used)
        return cache[n];

    /* --------------------------------------------------------------------- */
    /*  Ensure the cache can hold up to index n (array size n+1).             */
    /* --------------------------------------------------------------------- */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        /* seq_realloc ensures proper construction/destruction for seq_t   */
        cache = seq_realloc(cache, cache_cap, new_cap + 1); /* +1 for 1‑based index */
        cache_cap = new_cap + 1;
    }

    /* --------------------------------------------------------------------- */
    /*  Compute a(k) for all k from cache_used + 1 up to n.                   */
    /* --------------------------------------------------------------------- */
    for (size_t k = cache_used + 1; k <= (size_t)n; ++k) {
        /* ---- Ensure we have at least k primes (p_k needed) ---- */
        while (prime_cnt < k) {
            uint64_t p;
            if (prime_cnt == 0) p = 2;
            else p = next_prime(prime_arr[prime_cnt - 1]);

            if (prime_cnt >= prime_cap) {
                size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                uint64_t *tmp = (uint64_t*)realloc(prime_arr, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_arr = tmp;
                prime_cap = new_cap;
            }
            prime_arr[prime_cnt++] = p;
        }

        /* ---- Length of current block is the k-th prime p_k               */
        uint64_t block_len = prime_arr[k - 1];

        /* ---- Extend digits_buf until we have enough digits for this block */
        while (buf_len < curr_offset + block_len) {
            /* If we need more primes than currently generated, generate them */
            if (used_prime_idx >= prime_cnt) {
                uint64_t p = next_prime(prime_arr[prime_cnt - 1]);

                if (prime_cnt >= prime_cap) {
                    size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                    uint64_t *tmp = (uint64_t*)realloc(prime_arr,
                                                       new_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    prime_arr = tmp;
                    prime_cap = new_cap;
                }
                prime_arr[prime_cnt++] = p;
            }

            /* Append digits of prime_arr[used_prime_idx] to digits_buf      */
            uint64_t p = prime_arr[used_prime_idx++];
            /* Convert prime to string (works for all sizes of uint64_t)     */
            char tmpbuf[32];
            int diglen = snprintf(tmpbuf, sizeof(tmpbuf), "%llu",
                                  (unsigned long long)p);
            if (diglen <= 0) abort();

            if (buf_len + (size_t)diglen > buf_cap) {
                size_t new_cap = (buf_cap == 0) ? 64 : buf_cap * 2;
                while (new_cap < buf_len + (size_t)diglen) new_cap *= 2;
                char *tmp = (char*)realloc(digits_buf, new_cap);
                if (!tmp) abort();
                digits_buf = tmp;
                buf_cap = new_cap;
            }
            memcpy(digits_buf + buf_len, tmpbuf, (size_t)diglen);
            buf_len += (size_t)diglen;
        }

        /* ---- Build the integer value from the requested block            */
        seq_t value = 0;
        bool leading = true;
        for (size_t pos = curr_offset; pos < curr_offset + block_len; ++pos) {
            unsigned char d = digit_value(digits_buf[pos]);
            if (leading && d == 0) {
                /* Skip leading zeroes */
                continue;
            }
            leading = false;
            value = value * 10 + (seq_t)d;
        }
        /* If the block consists solely of zeros, value stays 0            */

        cache[k] = value;
        curr_offset += block_len;
    }

    cache_used = (size_t)n;
    return cache[(size_t)n];
}
