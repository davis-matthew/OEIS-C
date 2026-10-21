/*
 * A213087  Concatenate the binary representations of the nonnegative integers
 *          and form successive terms by inserting a comma after each zero.
 * https://oeis.org/A213087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 1794, u64 n <= 9780,
 *            u128 n <= 9780, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A213087: a(n) is the decimal number formed by the binary substring
   that ends at each 0 in the infinite stream obtained by concatenating
   the binary representations of 0,1,2,3,… .  The digits of the substring
   are 0‑and‑1, so the term is that binary string interpreted as a decimal
   integer (e.g. “110” → 110(10), “1110” → 1110(10)). */

/* Warning: the sequence is generated lazily.  The implementation keeps
   a static cache and a state machine that scans the binary stream bit by
   bit until the requested term is produced. */

#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* static cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* state of stream generator */
    static idx_t next_i = 0;     /* integer being processed */
    static int   next_bit = -1;  /* next bit position to read (-1 = need init) */
    static seq_t cur_term = 0;   /* current term being built */

    if (n == 0) return 0; /* undefined for n==0 but harmless */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* initialise the bit index for the next integer if needed */
        if (next_bit < 0) {
            if (next_i == 0) {
                next_bit = 0;   /* integer 0 has only one bit: “0” */
            } else {
                next_bit = (int)floor_log2_uint64(next_i);
            }
        }

        /* fetch the current bit */
        int b = (next_i == 0) ? 0 : (int)((next_i >> next_bit) & 1);

        /* advance the bit pointer for the next call */
        next_bit--;
        if (next_bit < 0) {          /* integer finished */
            ++next_i;
            next_bit = -1;            /* will initialise on next iteration */
        }

        /* accumulate the current term in decimal form:
           each new binary digit becomes a new decimal digit.
           cur_term = cur_term*10 + b; */
        cur_term = cur_term * 10 + (seq_t)b;

        /* a zero ends a term */
        if (b == 0) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 256;
                seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!p) abort();      /* out of memory */
                cache = p;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur_term;
            cur_term = 0;            /* reset for next term */
        }
    }

    return cache[n - 1];
}
