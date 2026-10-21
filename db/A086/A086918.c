/*
 * A086918  Numbers with at least three digits such that the absolute
 *          differences between successive digits forms a palindrome (when
 *          leading zeros are omitted).
 * https://oeis.org/A086918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force enumeration of integers ≥100 whose
   absolute differences between successive digits form a palindrome
   after trimming leading zeros. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached terms */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 99;        /* last candidate examined */

    if (cache == NULL) {           /* first call – allocate cache */
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        next++;                    /* candidate number */

        /* Extract digits in reverse order (least‑significant first) */
        int digits[128];
        size_t dlen = 0;
        seq_t x = next;
        while (x != 0) {
            digits[dlen++] = (int)(x % (seq_t)10);
            x /= (seq_t)10;
        }
        if (dlen < 3) continue;    /* need at least 3 digits */

        /* Compute absolute differences in the correct (most‑to‑least)
           digit order.  diffs[i] corresponds to |digit_{i}-digit_{i+1}|. */
        int diffs[127];
        size_t dcount = 0;
        for (int idx = (int)dlen - 1; idx > 0; idx--) {
            int a = digits[idx];
            int b = digits[idx - 1];
            int d = a - b;
            if (d < 0) d = -d;
            diffs[dcount++] = d;
        }

        /* Trim leading zeros from the difference sequence */
        size_t start = 0;
        while (start < dcount && diffs[start] == 0) start++;

        size_t len = dcount - start;   /* length after trimming */
        int ok = 1;
        if (len > 1) {                 /* check palindrome */
            for (size_t i = 0; i + i + 1 < len; i++) {
                if (diffs[start + i] != diffs[start + len - 1 - i]) {
                    ok = 0;
                    break;
                }
            }
        }
        /* len==0 or 1 is automatically a palindrome */

        if (!ok) continue;            /* not a valid term */

        /* Store the term, expanding the cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        cache[cache_len++] = next;
    }

    return cache[n - 1];
}
