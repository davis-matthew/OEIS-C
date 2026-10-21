/*
 * A077371  Fibonacci numbers whose internal digits form a Fibonacci number.
 *          Equivalently, Fibonacci numbers from which deleting the MSD and
 *          LSD leaves a Fibonacci number.
 * https://oeis.org/A077371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   a(n) = Fibonacci number F such that the integer obtained by *
 *            deleting the most significant and least significant digit of F
 *            is also a * Fibonacci number (OEIS A077371). This function
 *            iteratively generates * Fibonacci numbers and collects those
 *            that satisfy the property
 * Range:     u8 n <= 12, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   digit_len, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/floor_sqrt.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Test whether a sequence value is a Fibonacci number using the classic
 * sqrt test: 5*n^2 ± 4 is a perfect square. */
static bool is_fib_seq_t(seq_t m)
{
    const seq_t five  = 5;
    const seq_t four  = 4;
    seq_t sq = five * m * m + four;          /* 5*m^2 + 4 */
    seq_t r  = floor_sqrt(sq);
    if (r * r == sq) return true;

    sq = five * m * m - four;                /* 5*m^2 - 4 */
    r  = floor_sqrt(sq);
    return (r * r == sq);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* out‑of‑range guard – sequence starts at 1 */

    /* Static cache of discovered terms. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    static seq_t fib_prev = 0;  /* F0 */
    static seq_t fib_curr = 1;  /* F1 */

    /* Initialise cache once. */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        /* Compute internal digits of fib_prev. */
        int d = digit_len(fib_prev);
        seq_t trimmed;
        if (d <= 2) {
            trimmed = 0;                     /* no internal digits → 0 */
        } else {
            seq_t pow10_d1 = pow10((idx_t)(d - 1));
            seq_t leading  = fib_prev / pow10_d1;
            seq_t rest     = fib_prev - leading * pow10_d1;
            trimmed        = rest / 10;       /* delete last digit */
        }

        if (is_fib_seq_t(trimmed)) {
            /* Store qualifying Fibonacci number. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) { free(cache); /* abort on OOM */ return 0; }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = fib_prev;
        }

        /* Advance to next Fibonacci number. */
        seq_t next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next;
    }

    return cache[n - 1];
}
