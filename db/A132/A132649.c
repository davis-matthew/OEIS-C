/*
 * A132649  Lucky number L(n) raised to power L(n).
 * https://oeis.org/A132649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(n)^L(n) (L(n) = n-th lucky number)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, abort */
#include <string.h>   /* memcpy */
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

/* Cached lucky numbers: array of 64‑bit unsigned integers. */
static uint64_t *lucky_mem = NULL;
static size_t lucky_mem_size = 0;

/* Ensure we have at least 'require' lucky numbers in lucky_mem. */
static void ensure_lucky(size_t require)
{
    if (require <= lucky_mem_size)             /* already enough */
        return;

    /* Rough upper bound for the needed value (n log n). */
    size_t limit = require * 20 + 10;
    while (1) {
        /* Generate the list of odd numbers up to 'limit'. */
        size_t num_odds = (limit + 1) / 2;
        uint64_t *list = (uint64_t *)malloc(num_odds * sizeof(uint64_t));
        if (!list) abort();                     /* out of memory */

        for (size_t i = 0; i < num_odds; ++i)
            list[i] = 1 + 2 * i;                 /* 1,3,5,… */

        /* Perform the lucky–number sieve in place. */
        size_t len = num_odds;
        for (size_t i = 1; i < len; ++i) {       /* skip index 0 (value 1) */
            uint64_t step = list[i];
            if (step > len)                     /* no more eliminations */
                break;

            size_t newlen = 0;
            for (size_t j = 0; j < len; ++j)
                if ((j + 1) % step != 0)
                    list[newlen++] = list[j];
            len = newlen;
        }

        /* If we now have enough lucky numbers, store them. */
        if (len >= require) {
            uint64_t *new_mem = (uint64_t *)malloc(len * sizeof(uint64_t));
            if (!new_mem) abort();
            memcpy(new_mem, list, len * sizeof(uint64_t));

            free(lucky_mem);
            lucky_mem   = new_mem;
            lucky_mem_size = len;

            free(list);
            return;
        }

        /* Not enough yet – double the range and try again. */
        free(list);
        limit *= 2;
    }
}

/* Return the n‑th lucky number (1‑based). */
static seq_t nth_lucky(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n = 0, harmless */
    ensure_lucky((size_t)n);
    return (seq_t)lucky_mem[(size_t)n - 1];
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    seq_t l = nth_lucky(n);
    return big_pow(l, l);
}
