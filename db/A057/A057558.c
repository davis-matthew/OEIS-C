/*
 * A057558  Lexicographic ordering of MxMxMxM, where M={0,1,2,...}.
 * https://oeis.org/A057558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   lexicographic ordering of MxMxMxM (M = {0,1,2,…})
 * Range:     u8 n <= 140, u16 n <= 9089, u32 n <= 9089, u64 n <= 9089,
 *            u128 n <= 9089, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence obtained by flattening the tuples (a,b,c,d) in
   lexicographic order, sorted first by the sum s = a+b+c+d,
   then lexicographically by (a,b,c,d).  The nth term is the
   ((n-1)%4)‑th coordinate of the ceil(n/4)-th tuple. */

#include <stdint.h>

static inline seq_t binom_small(unsigned long long n, int k)
{
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; treat n<1 as 0 */

    /* Rank of the tuple (1‑based) */
    idx_t t = (n + 3) / 4;                /* ceil(n/4) */

    /* Find minimal sum s such that cumulative count >= t */
    idx_t s = 0;
    seq_t cumulative = 0;
    seq_t ways;
    while (1) {
        ways = binom_small((unsigned long long)(s + 3), 3);
        cumulative += ways;
        if (cumulative >= t) break;
        ++s;
    }

    seq_t prev_cumulative = cumulative - ways;   /* < t */
    seq_t rank_in_s = t - prev_cumulative;       /* 1‑based rank within sum s */

    /* Unrank the tuple of sum s */
    seq_t rem = (seq_t)s;
    seq_t a_val = 0, b_val = 0, c_val = 0, d_val = 0;
    seq_t k = rank_in_s;

    /* Determine a */
    for (idx_t a = 0; a <= rem; ++a) {
        seq_t cnt_a = binom_small((unsigned long long)(rem - a + 2), 2);
        if (k > cnt_a) {
            k -= cnt_a;
        } else {
            a_val = a;
            rem -= a;
            break;
        }
    }

    /* Determine b */
    for (idx_t b = 0; b <= rem; ++b) {
        seq_t cnt_b = rem - b + 1;  /* number of (c,d) pairs for this b */
        if (k > cnt_b) {
            k -= cnt_b;
        } else {
            b_val = b;
            rem -= b;
            break;
        }
    }

    /* Determine c */
    for (idx_t c = 0; c <= rem; ++c) {
        if (k > 1) {
            k -= 1;
        } else {
            c_val = c;
            rem -= c;
            break;
        }
    }

    d_val = rem;  /* remaining value */

    /* Return the desired coordinate */
    idx_t comp = (n - 1) % 4;
    if (comp == 0) return a_val;
    if (comp == 1) return b_val;
    if (comp == 2) return c_val;
    return d_val;
}
