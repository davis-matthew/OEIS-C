/*
 * A079652  Prime numbers using only the curved digits 0, 3, 6, 8 and 9.
 * https://oeis.org/A079652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Computes a(n) = the n-th prime whose decimal representation uses
 *            only the digits 0,3,6,8,9 (intersection of A000040 and A072960)
 * Range:     u8 n <= 3, u16 n <= 116, u32 n <= 11740, u64 n <= 11740,
 *            u128 n <= 11740, big any n
 * Validated: standard, pass, 61 terms through n = 9934 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Digit list in ascending order; leading digit cannot be 0. */
static const uint8_t DIGITS[5] = {0, 3, 6, 8, 9};

static seq_t *CACHE = NULL;          /* Array holding computed terms */
static size_t  CACHE_CAP = 0;        /* Allocated capacity of CACHE          */
static idx_t   CACHE_CNT = 0;        /* Number of terms stored so far        */
static size_t  NEXT_LEN  = 1;        /* Next number of digits to generate    */

/* Helper that generates all numbers of a given digit length
   and stores the primes into CACHE. */
static void generate_len(size_t len)
{
    /* Index array holding position in DIGITS for each digit:
       idx[0] is the leading digit (must be >=1).
       idx[i] for i>0 can be 0..4.                                  */
    size_t idx[32] = {0};
    /* Initialise leading digit to 1 (digit 3). */
    idx[0] = 1;
    /* Others start at 0 (digit 0). */

    for (size_t i = 1; i < len; ++i) idx[i] = 0;

    for (;;)
    {
        /* Build the number from the digit indices. */
        seq_t value = 0;
        for (size_t i = 0; i < len; ++i)
            value = value * 10 + DIGITS[idx[i]];

        /* Check for primality. */
        if (is_prime_big(value))
        {
            if (CACHE_CNT >= CACHE_CAP)
                grow_seq(&CACHE, &CACHE_CAP, CACHE_CNT + 1);
            CACHE[CACHE_CNT++] = value;
        }

        /* Increment the rightmost position. */
        size_t pos = len - 1;
        while (pos > 0 && (++idx[pos]) >= 5)
        {
            idx[pos] = 0;
            --pos;
        }
        if (pos == 0)        /* leading digit overflow */
        {
            if (++idx[0] >= 5)   /* finished all combinations of this length */
                break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* Undefined for offset 0, but harmless */

    /* Extend the cache until it contains at least n terms. */
    while (CACHE_CNT < n)
    {
        generate_len(NEXT_LEN);
        ++NEXT_LEN;
    }

    return CACHE[n - 1];
}
