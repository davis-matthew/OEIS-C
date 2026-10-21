/*
 * A073925  Powers of 2 rearranged such that every partial sum (n>1) is
 *          composite.
 * https://oeis.org/A073925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest unused power of two such that the sum of
 *            the first n terms is composite (greedy construction, a(1)=1)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   isPrimeGeneric, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t exp); */
#include "../utils/isPrimeGeneric.h"/* bool isPrimeGeneric(seq_t n); */

static bool is_composite_seq(seq_t n)
{
    /* 2 and 3 are prime, all smaller sums are not composite */
    if (n <= 3) return false;
    return !isPrimeGeneric(n);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0 */

    /* ----- static memoisation state ----- */
    static seq_t *seq = NULL;          /* array of generated terms */
    static size_t seq_len = 0;         /* how many terms we have */
    static size_t seq_cap = 0;         /* capacity of seq array        */
    static seq_t curr_sum = 0;         /* sum of all terms so far      */

    static uint8_t *used = NULL;       /* bit‑style flags for used exponents */
    static size_t used_cap = 0;        /* number of exponent slots          */

    /* ---------- initialisation on first call ---------- */
    if (seq_len == 0) {
        /* first term a(1) = 1 (= 2^0) */
        seq_cap = 1;
        seq = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        if (!seq) abort();
        seq[0] = pow2(0);
        seq_len = 1;
        curr_sum = seq[0];

        used_cap = 1;
        used = (uint8_t *)calloc(used_cap, sizeof(uint8_t));
        if (!used) abort();
        used[0] = 1;        /* exponent 0 already used */
    }

    /* ---------- return cached value if available ---------- */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n - 1];

    /* ---------- build sequence up to index n ---------- */
    size_t target = (size_t)n;
    while (seq_len < target) {
        /* Search for the smallest unused exponent e that makes
           the new partial sum composite. */
        idx_t e = 0;
        for (;;) {
            /* grow the used array if necessary */
            if ((size_t)e >= used_cap) {
                size_t new_cap = used_cap ? used_cap * 2 : 1;
                while ((size_t)e >= new_cap) new_cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
                if (!tmp) abort();
                memset(tmp + used_cap, 0,
                       (new_cap - used_cap) * sizeof(uint8_t));
                used = tmp;
                used_cap = new_cap;
            }
            if (used[e]) {          /* already used power of two */
                ++e;
                continue;
            }

            seq_t candidate_sum = curr_sum + pow2(e);
            if (is_composite_seq(candidate_sum)) {
                /* we found the correct exponent */
                if (seq_len >= seq_cap) {
                    size_t new_cap = seq_cap ? seq_cap * 2 : 1;
                    while (seq_len >= new_cap) new_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    seq = tmp;
                    seq_cap = new_cap;
                }
                seq[seq_len++] = pow2(e);
                curr_sum = candidate_sum;
                used[e] = 1;
                break;     /* advance to build next term */
            } else {
                ++e;          /* try next unused exponent */
            }
        }
    }

    return seq[(size_t)n - 1];
}
