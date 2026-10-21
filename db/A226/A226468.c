/*
 * A226468  Numbers in which each digit equals the sum (mod 10) of the other
 *          digits.
 * https://oeis.org/A226468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the sequence consists of all positive integers in which * each
 *            digit equals the sum modulo 10 of the other digits
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* popcount for 64‑bit values */
static inline int popcnt64(uint64_t v)
{
    return __builtin_popcountll(v);
}

/* comparison routine for qsort on seq_t */
static int cmp_seq_t(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;
}

/*  OEIS A226468  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* static state */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t maxlen_generated = 0;   /* longest length produced so far */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        ++maxlen_generated;                       /* next length to generate */
        uint64_t L = maxlen_generated;             /* number of digits */

        /* generate for each pair x,(x+5) where x=0..4 */
        for (int x = 0; x <= 4; ++x) {
            int y = x + 5;                         /* the other digit of the pair */

            /* enumerate all binary strings of length L */
            /* each bit denotes whether the position holds digit y (bit=1) or x (bit=0) */
            uint64_t total_masks = (L == 64) ? 0 : (1ULL << L);
            if (L == 64) continue;                /* avoid undefined shift – never needed */

            for (uint64_t mask = 0; mask < total_masks; ++mask) {
                /* leading digit must not be 0 (only the 0,5 pair uses zero) */
                if (x == 0 && ((mask & 1ULL) == 0)) continue;

                int b = popcnt64(mask);              /* number of y's in this combination */
                int nlen = (int)L;
                bool ok = false;

                /* check the algebraic constraint for this pair and length */
                switch (x) {
                    case 0:                         /* pair (0,5) – b must be even and at least one 5 */
                        if ((b & 1) == 0 && b >= 1) ok = true;
                        break;
                    case 1: case 3:                 /* pairs (1,6) and (3,8) */
                        if ((b & 1) == 0) {
                            if (nlen % 10 == 2) ok = true;     /* b even → n ≡ 2 (mod 10) */
                        } else {
                            if (nlen % 10 == 7) ok = true;     /* b odd  → n ≡ 7 (mod 10) */
                        }
                        break;
                    case 2: case 4:                 /* pairs (2,7) and (4,9) */
                        if ((b & 1) == 0) {
                            if (nlen % 5 == 2) ok = true;      /* b even → n ≡ 2 (mod 5) */
                        }
                        break;
                }
                if (!ok) continue;

                /* construct the number according to the mask */
                seq_t val = 0;
                for (int pos = 0; pos < nlen; ++pos) {
                    int digit = ((mask >> pos) & 1ULL) ? y : x;
                    val = val * (seq_t)10 + (seq_t)digit;
                }

                /* add to the list (grow if necessary) */
                if (terms_len == terms_cap) {
                    size_t newcap = terms_cap ? terms_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                    if (!tmp) abort();                /* allocation failed */
                    terms = tmp;
                    terms_cap = newcap;
                }
                terms[terms_len++] = val;
            }
        }

        /* keep the array in ascending order – all new numbers are larger
           than all previously generated ones, but they are not
           necessarily sorted within the new length, so we sort the whole list. */
        qsort(terms, terms_len, sizeof(seq_t), cmp_seq_t);
    }

    return terms[n - 1];
}
