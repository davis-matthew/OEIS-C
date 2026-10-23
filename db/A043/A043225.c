/*
 * A043225  Numbers k such that 1 and 2 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   digit_len, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043225: numbers k such that the decimal representation of k
 * contains the substring 12 or 21, while the preceding integer k‑1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_digits.h"   /* for get_digits() */
#include "../utils/digit_len.h"    /* for digit_len() */

/* Helper: test whether a decimal representation of x contains
 * the adjacent digits 1 and 2 in either order. */
static bool contains12or21(seq_t x)
{
    size_t d = digit_len(x);
    if (d < 2) return false;
    uint8_t *buf = (uint8_t*)malloc(d);
    if (!buf) return false;          /* failed allocation – silently treat as false */
    size_t len = 0;
    get_digits(x, buf, &len);        /* len == d */
    for (size_t i = 0; i + 1 < len; ++i)
    {
        if ((buf[i] == 1 && buf[i+1] == 2) || (buf[i] == 2 && buf[i+1] == 1))
        {
            free(buf);
            return true;
        }
    }
    free(buf);
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached terms */
    static size_t cap   = 0;         /* capacity of cache */
    static size_t len   = 0;         /* number of terms stored */
    static seq_t last_checked = 11;  /* last integer examined */
    static bool last_contains = false; /* whether last_checked contains 12/21 */

    if (n == 0) return 0;            /* undefined for n==0, return 0 */

    /* Extend cache until it contains at least n terms. */
    while (len < (size_t)n)
    {
        seq_t k = last_checked + 1;
        bool contains_k = contains12or21(k);

        if (contains_k && !last_contains)
        {
            if (len == cap)          /* grow cache */
            {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = k;
        }

        last_contains = contains_k;
        last_checked = k;
    }

    return cache[(size_t)n - 1];
}
