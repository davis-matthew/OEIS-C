/*
 * A110893  Numbers with a semiprime number of prime divisors (counted with
 *          multiplicity).
 * https://oeis.org/A110893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9921 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the nth integer k such that bigomega(k) is a semiprime
   (product of two primes, possibly equal). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* check whether a small integer omega is a semiprime */
static inline bool is_semiprime_omega(uint8_t omega)
{
    if (omega < 2) return false;
    uint32_t cnt = 0;
    uint32_t n = omega;
    for (uint32_t p = 2; p * p <= n; ++p)
    {
        while (n % p == 0)
        {
            ++cnt;
            n /= p;
            if (cnt > 2) return false;
        }
    }
    if (n > 1) ++cnt;
    return cnt == 2;
}

/* static storage for the sequence terms */
static seq_t *seq      = NULL;   /* array of found terms */
static idx_t  seq_size = 0;      /* number of stored terms */
static size_t seq_cap  = 0;      /* capacity of seq array */

/* static storage for sieve */
static uint32_t *spf      = NULL; /* smallest prime factor */
static uint8_t  *bigomega = NULL; /* total number of prime factors */
static size_t    max_limit = 0;    /* size up to which spf/bigomega are valid */

/* index up to which we have already scanned for terms */
static size_t last_scanned = 1;

/* extend sieve and bigomega arrays up to new_limit (inclusive) */
static void extend_sieve(size_t new_limit)
{
    if (new_limit <= max_limit) return;

    /* allocate new buffers */
    uint32_t *new_spf = (uint32_t*)calloc(new_limit + 1, sizeof(uint32_t));
    if (!new_spf) return; /* allocation failed – unlikely in tests */

    /* linear sieve variant (sufficient for the required size) */
    for (size_t i = 2; i <= new_limit; ++i)
    {
        if (new_spf[i] == 0)
        {
            new_spf[i] = (uint32_t)i;
            if (i * (size_t)i <= new_limit)
            {
                for (size_t j = i * i; j <= new_limit; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* compute bigomega using the spf array */
    uint8_t *new_bigomega = (uint8_t*)calloc(new_limit + 1, sizeof(uint8_t));
    if (!new_bigomega) { free(new_spf); return; }
    new_bigomega[0] = 0;
    new_bigomega[1] = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        new_bigomega[i] = new_bigomega[i / new_spf[i]] + 1;

    /* free old data */
    if (spf)   free(spf);
    if (bigomega) free(bigomega);

    spf = new_spf;
    bigomega = new_bigomega;
    max_limit = new_limit;
}

/* fill seq array until it contains at least n terms */
static void ensure_sequence(idx_t n)
{
    if ((idx_t)seq_size >= n) return;

    /* start with a reasonable limit, grow exponentially if needed */
    size_t limit = 100000;
    if (limit < max_limit) limit = max_limit * 2; /* avoid shrinking */

    while ((idx_t)seq_size < n)
    {
        if (limit < max_limit || limit <= last_scanned)
            limit = last_scanned * 2 + 100; /* bump further */

        extend_sieve(limit);

        for (size_t i = last_scanned + 1; i <= limit; ++i)
        {
            if (is_semiprime_omega(bigomega[i]))
            {
                /* grow seq array if necessary */
                if (seq_size == 0)
                {
                    seq = mem_new(seq_t, 256);
                    seq_cap = 256;
                    seq[0] = (seq_t)i;
                    seq_size = 1;
                }
                else if ((size_t)seq_size >= seq_cap)
                {
                    seq_cap *= 2;
                    seq = mem_resize(seq_t, seq, seq_cap);
                    seq[seq_size++] = (seq_t)i;
                }
                else
                {
                    seq[seq_size++] = (seq_t)i;
                }
            }
        }
        last_scanned = limit;
        limit *= 2; /* double for next iteration if needed */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1 – undefined for n==0 */
    ensure_sequence(n);
    return seq[n - 1];
}
