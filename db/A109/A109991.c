/*
 * A109991  Numbers k such that the string 88k is prime.
 * https://oeis.org/A109991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 72 terms through n = 2130 (2026-11-03)
 * Helpers:   is_prime_seq, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A109991: numbers k such that the decimal string "88" concatenated with k is prime. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

/* Computes the nth element of OEIS sequence A109991. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence indices start at 1 */

    /* Memoisation of found terms. */
    static seq_t  *klist = NULL;          /* array of found k values      */
    static idx_t   kcap  = 0;             /* current allocated capacity  */
    static idx_t   ksize = 0;             /* number of stored values     */
    static idx_t   next  = 1;             /* next integer to test        */

    /* Generate terms until we have at least n entries. */
    while (ksize < n) {
        seq_t cand = (seq_t)next;                     /* current k candidate   */
        idx_t d    = num_digits_idx(next);              /* digits of k           */
        seq_t val  = (seq_t)88 * pow10(d) + cand;     /* 88 concatenated with k */

        if (is_prime_seq(val)) {
            /* Grow storage if needed. */
            if (ksize == kcap) {
                idx_t newcap = kcap ? kcap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, klist, newcap);
                if (!tmp) return 0;                  /* allocation failure   */
                klist = tmp;
                kcap  = newcap;
            }
            klist[ksize++] = cand;
        }
        ++next;                                        /* advance to next k     */
    }

    return klist[n - 1];
}
