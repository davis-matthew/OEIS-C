/*
 * A213308  Numbers with exactly one nonprime substring (substrings with
 *          leading zeros are considered to be nonprime).
 * https://oeis.org/A213308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A213308 (numbers with exactly one nonprime
 * substring).  The function generates terms on demand, caching them in
 * a static table.  The computation uses only integer arithmetic and
 * the supplied isPrimeTrial() utility. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms = NULL;   /* array of found terms */
    static size_t used   = 0;     /* number of terms stored */
    static size_t cap    = 0;     /* capacity of the array */
    static idx_t  cand   = 1;     /* next candidate number to test */

    /* allocate initial storage */
    if (!terms) {
        cap   = 32;
        terms = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* keep searching until we have at least n terms or a safe upper limit */
    while (used < (size_t)n && cand <= 10000) {
        /* build decimal digits (most significant first) */
        uint8_t  digs[20];
        size_t   len   = 0;
        idx_t    tmp   = cand;
        do {
            digs[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        } while (tmp);
        /* reverse to obtain most‑significant‑first order */
        for (size_t i = 0; i < len / 2; ++i) {
            uint8_t t = digs[i];
            digs[i]   = digs[len - 1 - i];
            digs[len - 1 - i] = t;
        }

        /* count non‑prime substrings */
        int nonprime = 0;
        for (size_t i = 0; i < len; ++i) {
            if (digs[i] == 0) {                 /* leading zero: all substrings non‑prime */
                nonprime += (int)(len - i);
                if (nonprime > 1) break;
                continue;
            }
            seq_t val = 0;
            for (size_t j = i; j < len; ++j) {
                val = val * 10 + (seq_t)digs[j];
                if (!isPrimeTrial(val)) {
                    ++nonprime;
                    if (nonprime > 1) break;
                }
            }
            if (nonprime > 1) break;
        }

        if (nonprime == 1) {
            /* store the term */
            if (used == cap) {
                cap <<= 1;
                terms = (seq_t*)realloc(terms, cap * sizeof(seq_t));
            }
            terms[used++] = (seq_t)cand;
        }
        ++cand;
    }

    /* if index out of range, return 0 (undefined behaviour allowed outside offset) */
    if (n > used) return (seq_t)0;
    return terms[n - 1];
}
