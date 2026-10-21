/*
 * A076492  Number of common decimal digits of n! and (n+1)!.
 * https://oeis.org/A076492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal digits that appear in both n! and
 *            (n+1)!
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   popcount10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/popcount10.h"

/* compute the sequence */
seq_t a(idx_t n)
{
    /* arbitrary‑precision factorial in base 10, little‑endian */
    static uint8_t *fact = NULL;      /* array of decimal digits */
    static size_t fact_cap = 0;       /* allocated capacity */
    static size_t fact_len = 0;       /* current length */

    /* cache of previously computed terms */
    static seq_t *cache = NULL;       /* cache[1..cur_n] */
    static size_t cache_cap = 0;      /* allocated size */
    static idx_t cur_n = 1;           /* n of the current factorial (fact = n!) */
    static uint16_t mask_cur = 0;     /* digit mask of current factorial */

    /* lazy initialization */
    if (!fact)
    {
        fact_cap = 40000;                 /* enough for 10000! (~35660 digits) */
        fact = (uint8_t*)malloc(fact_cap);
        fact_len = 1;
        fact[0] = 1;                     /* 1! = 1 */
        mask_cur = 0;
        mask_cur |= 1U << 1;              /* digit 1 */
    }

    /* ensure cache can hold up to n (inclusive) */
    if (cache == NULL)
    {
        cache_cap = (size_t)n + 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    else if ((size_t)n + 1 > cache_cap)
    {
        cache_cap = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute terms until we reach n */
    while (cur_n <= n)
    {
        /* multiply factorial by (cur_n + 1) to get (cur_n + 1)! */
        idx_t mult = cur_n + 1;
        uint32_t carry = 0;
        for (size_t i = 0; i < fact_len; ++i)
        {
            uint32_t prod = fact[i] * (uint32_t)mult + carry;
            fact[i] = prod % 10;
            carry    = prod / 10;
        }
        while (carry)
        {
            if (fact_len >= fact_cap) return 0; /* safety, should not happen */
            fact[fact_len++] = carry % 10;
            carry            /= 10;
        }

        /* compute digit mask of the new factorial */
        uint16_t mask_next = 0;
        for (size_t i = 0; i < fact_len; ++i)
            mask_next |= 1U << fact[i];

        /* a(cur_n) = number of common decimal digits of n! and (n+1)! */
        cache[cur_n] = (seq_t)popcount10(mask_cur & mask_next);

        /* step forward */
        cur_n++;
        mask_cur = mask_next;
    }

    return cache[(size_t)n];
}
