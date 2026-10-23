/*
 * A043866  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 8 mod 9.
 * https://oeis.org/A043866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043866: Numbers n such that the number of runs in the base‑4
   representation of n is congruent to 8 (mod 9). The algorithm finds
   the n‑th such number by successive enumeration with memoisation. */
#include <stdlib.h>
#include <stdbool.h>

/* static cache for already found terms */
static seq_t *cache    = NULL;  /* array of seq_t values          */
static idx_t  len      = 0;     /* how many terms are stored      */
static size_t cap      = 0;     /* current allocation size        */
static seq_t  candidate = 0;    /* last integer that has been scanned */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, so this case is unused */

    /* Return cached value if already computed */
    if (n <= len)
        return cache[n - 1];

    /* Ensure capacity for at least n terms */
    if (len < n) {
        if ((size_t)n > cap) {
            size_t newcap = (cap == 0) ? 8 : cap;
            while (newcap < (size_t)n) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();           /* out of memory */
            cache = tmp;
            cap   = newcap;
        }

        /* Keep scanning integers until we have found n terms. */
        while (len < n) {
            /* Move to next integer candidate */
            candidate += 1;

            /* Count runs of digits in base‑4 representation */
            int runs = 0;
            seq_t t = candidate;
            if (t != 0) {
                seq_t last_digit = t % 4;
                t /= 4;
                runs = 1;
                while (t != 0) {
                    seq_t d = t % 4;
                    if (d != last_digit) {
                        ++runs;
                        last_digit = d;
                    }
                    t /= 4;
                }
            }

            /* If runs ≡ 8 (mod 9), store the candidate */
            if ((runs % 9) == 8)
                cache[len++] = candidate;
        }
    }

    return cache[n - 1];
}
