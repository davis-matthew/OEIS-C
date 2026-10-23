/*
 * A127011  a(1)=1. a(n) = the numerator of the sum of the reciprocals of the
 *          earlier terms of the sequence which divide n.
 * https://oeis.org/A127011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127011: a(1)=1.
   a(n) = numerator of ∑_{k<n, a(k)|n} 1/a(k)   (reduced fraction) */

#include <stdint.h>
#include <stdlib.h>

static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* ------------------- static data ---------------------------------- */
    static seq_t *seq = NULL;          /* array of computed terms   */
    static size_t seq_cap = 0;         /* capacity of seq array    */
    static unsigned int *cnt = NULL;   /* occurrences of each value*/
    static size_t cnt_cap = 0;         /* capacity of cnt array    */
    static size_t computed = 0;        /* highest index computed   */
    static int initialized = 0;

    if (!initialized) {
        /* initial capacity for a(1) */
        seq_cap = 2;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 0;               /* dummy */
        seq[1] = (seq_t)1;        /* a(1) */
        computed = 1;

        /* allocation for value counts up to at least 1 */
        cnt_cap = 2;
        cnt = (unsigned int *)calloc(cnt_cap, sizeof(unsigned int));
        cnt[1] = 1;              /* one occurrence of value 1 */

        initialized = 1;
    }

    /* ------------------- grow seq array if needed --------------------- */
    if ((size_t)n >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (seq_cap < new_cap)
            memset(seq + seq_cap, 0, (new_cap - seq_cap) * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* ------------------- compute missing terms ----------------------- */
    for (idx_t cur = (idx_t)(computed + 1); cur <= n; ++cur) {
        seq_t num = 0;   /* numerator of current sum   */
        seq_t den = 1;   /* denominator of current sum */

        /* iterate divisors d of cur */
        for (idx_t d = 1; d * d <= cur; ++d) {
            if (cur % d == 0) {
                /* divisor d */
                unsigned int c1 = (d < cnt_cap) ? cnt[d] : 0;
                if (c1) {
                    seq_t nd = num * (seq_t)d + den * (seq_t)c1;
                    seq_t dd = den * (seq_t)d;
                    seq_t g = gcd_seq(nd, dd);
                    num = nd / g;
                    den = dd / g;
                }

                /* paired divisor cur/d (skip if equal to d) */
                idx_t d2 = cur / d;
                if (d2 != d) {
                    unsigned int c2 = (d2 < cnt_cap) ? cnt[d2] : 0;
                    if (c2) {
                        seq_t nd = num * (seq_t)d2 + den * (seq_t)c2;
                        seq_t dd = den * (seq_t)d2;
                        seq_t g = gcd_seq(nd, dd);
                        num = nd / g;
                        den = dd / g;
                    }
                }
            }
        }

        seq_t val = num;  /* numerator of the reduced sum */
        seq[(size_t)cur] = val;

        /* update count of this value */
        if ((size_t)val >= cnt_cap) {
            size_t new_cap = cnt_cap ? cnt_cap * 2 : 16;
            if (new_cap <= (size_t)val) new_cap = (size_t)val + 1;
            cnt = (unsigned int *)realloc(cnt, new_cap * sizeof(unsigned int));
            if (cnt_cap > 0)
                memset(cnt + cnt_cap, 0, (new_cap - cnt_cap) * sizeof(unsigned int));
            cnt_cap = new_cap;
        }
        cnt[(size_t)val] += 1;
    }
    computed = (size_t)n;
    return seq[(size_t)n];
}
