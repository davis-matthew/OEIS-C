/*
 * A136823  Numbers k such that k and k^2 use only the digits 0, 1, 2, 5 and
 *          6.
 * https://oeis.org/A136823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 160, u64 n <= 1355,
 *            u128 n <= 1355, big any n
 * Helpers:   digit_allowed, sq_digits_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A136823: a(n) is the n‑th non‑negative integer k such that the
 * decimal digits of k and k² belong to the set {0,1,2,5,6}.
 * The implementation enumerates all integers composed only of those digits
 * (in increasing numeric order) and keeps those whose square also contains
 * only allowed digits.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_allowed.h"
#include "../utils/sq_digits_allowed.h"

/* allowed decimal digits */
static const int allowed_digits[5] = {0, 1, 2, 5, 6};

/* -------- dynamic cache for the sequence -------- */
static seq_t *SEQ_CACHE = NULL;   /* array of found terms */
static size_t SEQ_SIZE  = 0;      /* number of terms stored */
static size_t SEQ_CAP   = 0;      /* allocated capacity */

/* push a new term into the static cache */
static void add_term(seq_t v)
{
    if (SEQ_SIZE == SEQ_CAP) {
        SEQ_CAP = SEQ_CAP * 2 + 16;
        SEQ_CACHE = (seq_t *)realloc(SEQ_CACHE, SEQ_CAP * sizeof(seq_t));
    }
    SEQ_CACHE[SEQ_SIZE++] = v;
}

/* initialise cache with the first term, 0 */
static void init_cache(void)
{
    if (SEQ_CACHE == NULL) {
        SEQ_CAP = 16;
        SEQ_CACHE = (seq_t *)malloc(SEQ_CAP * sizeof(seq_t));
        if (SEQ_CACHE == NULL) return;      /* allocation failure, abort silently */
        SEQ_CACHE[0] = (seq_t)0;
        SEQ_SIZE = 1;
    }
}

/* recursive enumeration of k with exactly `len` decimal digits */
static void gen_len_rec(int pos, int len, seq_t current)
{
    if (pos == len) {
        seq_t sq = current * current;        /* for seq_t ⊆ 128‑bit or arbitrary‑prec */
        if (sq_digits_allowed(sq))
            add_term(current);
        return;
    }

    for (int i = 0; i < 5; ++i) {
        int d = allowed_digits[i];
        if (pos == 0 && d == 0) continue;   /* leading zero not allowed */
        seq_t next = current * 10 + (seq_t)d;
        gen_len_rec(pos + 1, len, next);
    }
}

/* generate all admissible numbers of a given decimal length */
static void generate_len(unsigned int len)
{
    gen_len_rec(0, (int)len, (seq_t)0);
}

/* next decimal length to be processed */
static unsigned NEXT_LEN = 1;

/* public API --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety; OEIS offset is 1 */

    init_cache();                            /* ensure the leading zero is present */

    while (SEQ_SIZE < (size_t)n) {
        generate_len(NEXT_LEN);
        ++NEXT_LEN;
    }
    return SEQ_CACHE[n - 1];
}
