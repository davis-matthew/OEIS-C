/*
 * A154487  a(n+1)-+a(n)=prime, a(1)=0,a(2)=7.
 * https://oeis.org/A154487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154487
 * a(1) = 0, a(2) = 7
 * For n > 2: let d = the smallest prime such that
 *     is_prime_big(2*a(n-1) + d) == true
 * then a(n) = a(n-1) + d
 * The sequence guarantees that for every consecutive pair
 * (a(k), a(k+1)) both the sum and difference are primes.  */

#include <stddef.h>                 /* size_t */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

static seq_t *seq_cache = NULL;   /* array storing a(1), a(2), ...      */
static size_t seq_size = 0;       /* number of terms currently computed */
static size_t seq_cap  = 0;       /* allocated capacity of seq_cache   */

/* Grow seq_cache to at least 'need' elements */
static inline void ensure_capacity(size_t need)
{
    if (seq_cap >= need)
        return;

    size_t newcap = seq_cap ? seq_cap << 1 : 4;
    while (newcap < need)
        newcap <<= 1;

    /* Allocate new array of seq_t */
    seq_t *new_arr = seq_alloc_array(newcap);

    /* Copy existing values */
    for (size_t i = 0; i < seq_size; ++i)
        new_arr[i] = seq_cache[i];

    /* Free old array if it existed */
    if (seq_cache)
        seq_free_array(seq_cache);

    seq_cache = new_arr;
    seq_cap  = newcap;
}

/* Initialise the sequence with the first two terms */
static inline void init_sequence(void)
{
    if (seq_size > 0)          /* already initialised */
        return;

    ensure_capacity(2);
    seq_cache[0] = (seq_t)0;    /* a(1) */
    seq_cache[1] = (seq_t)7;    /* a(2) */
    seq_size    = 2;
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined; oddsays n >= 1 */
        return (seq_t)0;

    ensure_capacity(n);
    init_sequence();

    /* Generate missing terms up to n */
    while (seq_size < (size_t)n)
    {
        seq_t cur = seq_cache[seq_size - 1];

        idx_t d = 2;             /* candidate difference */
        while (1)
        {
            if (is_prime_seq(d))            /* d must be prime      */
            {
                /* Check whether 2*cur + d is prime */
                seq_t sum = cur * (seq_t)2 + (seq_t)d;
                if (is_prime_big(sum))
                {
                    seq_t next = cur + (seq_t)d;
                    seq_cache[seq_size] = next;
                    ++seq_size;
                    break;
                }
            }
            ++d;                /* try next candidate */
        }
    }

    return seq_cache[n - 1];
}
