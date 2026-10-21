/*
 * A094934  Write binary digits of numbers 1, 2, 3, ... in a string; sequence
 *          gives the prime indexed digits.
 * https://oeis.org/A094934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the pₙ‑th binary digit (0‑indexed) in the concatenation
 *            of the binary representations of 1,2,3,…, where pₙ is the
 *            (n+1)‑th prime
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   get_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_len.h"

/* ----------  static storage ---------- */
static idx_t *prime_list   = NULL;  /* list of primes */
static size_t prime_cap    = 0;
static size_t prime_len    = 0;

static idx_t *cum_len      = NULL;  /* cumulative digit counts:
                                     cum_len[i] = total digits up to number i */
static size_t cum_cap      = 0;
static size_t cum_len_len  = 0;     /* number of used elements in cum_len
                                     (valid indices: 0 .. cum_len_len-1) */

static idx_t next_num      = 1;     /* next natural number to process */

static void grow_primes(size_t new_cap)
{
    prime_list = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    prime_cap = new_cap;
}

static void grow_cum_len(size_t new_cap)
{
    cum_len = (idx_t *)realloc(cum_len, new_cap * sizeof(idx_t));
    cum_cap = new_cap;
}

static bool is_prime_trial(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_len; ++i)
    {
        idx_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

static void ensure_primes(idx_t for_n)
{
    /* generate numbers until we have at least for_n+1 primes */
    if (prime_len > (size_t)for_n) return;

    /* initialise cumulative array if it does not exist yet */
    if (cum_len_len == 0)
    {
        cum_cap     = 16;
        cum_len     = (idx_t *)malloc(cum_cap * sizeof(idx_t));
        cum_len[0]  = 0;          /* dummy for number 0 */
        cum_len_len = 1;
    }

    /* enlarge prime_list capacity when needed */
    while (prime_cap == 0 || prime_cap < prime_len + 128)
    {
        grow_primes(prime_cap ? prime_cap << 1 : 128);
    }

    /* process numbers incrementally until enough primes are found */
    while (prime_len <= (size_t)for_n)
    {
        /* enlarge cum_len capacity when needed */
        if (cum_len_len == cum_cap)
            grow_cum_len(cum_cap ? cum_cap << 1 : 16);

        /* append next number's length to cumulative array */
        idx_t len = get_len(next_num);
        cum_len[cum_len_len] = cum_len[cum_len_len - 1] + len;
        ++cum_len_len;

        /* test primality */
        if (is_prime_trial(next_num))
            prime_list[prime_len++] = next_num;

        ++next_num;
    }
}

/* ----------  main sequence function ---------- */
seq_t a(idx_t n)
{
    /* ensure the first n+1 primes are ready */
    ensure_primes(n);

    /* position p of the n-th prime (0‑indexed) */
    idx_t p = prime_list[(size_t)n];

    /* binary search over cum_len to find the number containing position p */
    idx_t low  = 1; /* numbers start at 1 */
    idx_t high = (idx_t)(cum_len_len - 1);
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (cum_len[(size_t)mid] >= p)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t number = low;
    idx_t prev_cum = cum_len[(size_t)(number - 1)];
    idx_t offset  = p - prev_cum - 1;              /* 0‑based offset in binary(number) */
    idx_t len_of_number = get_len(number);
    idx_t shift = len_of_number - 1 - offset;      /* bit position from LSB */
    uint8_t digit = (uint8_t)((number >> shift) & 1);
    return (seq_t)digit;
}
