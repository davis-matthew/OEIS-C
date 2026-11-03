/*
 * A138566  Numbers n whose digits satisfy the following Diophantine equation:
 *          P = (X1* ... *Xr)/(X1+ ... +Xr), where P = prime number, Xi =
 *          digits of n.
 * https://oeis.org/A138566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer whose digits satisfy
   (digit_product)/(digit_sum) = prime.
   This implementation generates terms on demand and caches them
   for efficient retrieval in any order of calls. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* static cache for discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static uint64_t next_num = 1;  /* next number to test */

    /* generate terms until we have at least n of them */
    while (cache_sz < n)
    {
        uint64_t val = next_num++;
        seq_t sum = 0;
        seq_t prod = 1;

        /* compute product of digits and sum of digits */
        uint64_t tmp = val;
        while (tmp > 0)
        {
            uint64_t d = tmp % 10;
            tmp /= 10;
            sum += (seq_t)d;
            prod *= (seq_t)d;
            if (d == 0) {   /* product will be 0, skip early */
                prod = 0;
                break;
            }
        }

        /* check the Diophantine condition: prod = prime * sum */
        if (sum > 0 && prod % sum == 0)
        {
            seq_t prime = prod / sum;
            if (is_prime_big(prime))
            {
                /* store the original number into cache */
                append_candidate(&cache, &cache_sz, &cache_cap, (seq_t)val);
            }
        }
    }
    return cache[n - 1];
}
