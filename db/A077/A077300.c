/*
 * A077300  Partition the concatenation 1234567...of natural numbers into
 *          successive strings which are multiples of 7, all different and > 7
 *          (0 is never taken as the most significant digit).
 * https://oeis.org/A077300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   x_to_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A077300.
   It concatenates the natural numbers 1234567… and partitions it into
   minimal successive strings that are:
     * a multiple of 7
     * strictly larger than 7
     * never used before
   The function returns the n‑th such string (a(1) = 1234567891011). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/x_to_digits.h"

/* State kept by the implementation. */
struct A077300State {
    size_t nvals;    /* number of values already produced      */
    seq_t *vals;     /* array of produced values              */
    size_t vals_cap; /* capacity of vals array                */
    seq_t *used;     /* array of values seen before          */
    size_t used_cap; /* capacity of used array               */
    size_t used_cnt; /* number of used values                */
    idx_t cur_num;   /* current natural number to output      */
    char buf[32];    /* digits of cur_num                    */
    size_t buflen;   /* length of buf                        */
    size_t bufpos;   /* next position to read in buf         */
};

static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
    if (newcap < need) newcap = need;
#if defined(__cplusplus)
    *arr = reinterpret_cast<seq_t*>(realloc(*arr, newcap * sizeof(seq_t)));
#else
    *arr = realloc(*arr, newcap * sizeof(seq_t));
#endif
    *cap = newcap;
}

static bool contains(const seq_t *arr, size_t cnt, const seq_t val)
{
    for (size_t i = 0; i < cnt; ++i)
        if (arr[i] == val) return true;
    return false;
}

static char next_digit(struct A077300State *s)
{
    if (s->bufpos >= s->buflen) {
        s->buflen = x_to_digits(s->cur_num, s->buf);
        s->bufpos = 0;
        ++s->cur_num;
    }
    return s->buf[s->bufpos++];
}

seq_t a(idx_t n)
{
    static struct A077300State s = {0, NULL, 0, NULL, 0, 0, 1, {0}, 0, 0};

    if (n == 0) return 0;  /* undefined for n = 0, harmless */

    while (s.nvals < (size_t)n) {
        seq_t cand = 0;
        int mod7 = 0;
        while (true) {
            int d = next_digit(&s) - '0';
            cand = cand * 10 + (seq_t)d;
            mod7 = (mod7 * 10 + d) % 7;
            if (cand > (seq_t)7 && mod7 == 0 && !contains(s.used, s.used_cnt, cand))
                break;
        }
        grow_seq(&s.used, &s.used_cap, s.used_cnt + 1);
        s.used[s.used_cnt++] = cand;
        grow_seq(&s.vals, &s.vals_cap, s.nvals + 1);
        s.vals[s.nvals++] = cand;
    }
    return s.vals[(size_t)n - 1];
}
