/*
 * A038733  T(n,n-4), array T as in A038730.
 * https://oeis.org/A038733
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A038733: a(n) = sum_{k=0}^{n-4} F_{2k+2} * binom(n-k+2,6),
   where F_{m} is the Fibonacci sequence (F_0=0, F_1=1) and
   binom(m,6)=m!/(6!(m-6)!).  Equivalently,
   a(n) = coefficient of x^n in   x^4 / ((1-3x+x^2)(1-x)^7). */
#include <stdlib.h>

static seq_t *ak_table = NULL;  /* a_k = F_{2k+2} */
static size_t ak_len   = 0;     /* number of a_k already computed */

/* compute a(n) for n >= 4 */
seq_t a(idx_t n)
{
    if (n < 4)                  /* offset 4 – behaviour below is irrelevant */
        return (seq_t)0;

    /* required number of a_k values: k = 0 .. n-4 */
    idx_t r = n - 4;                     /* maximum k in the sum */
    size_t need = (size_t)(r + 1);        /* array length required */

    /* grow a_k table if necessary */
    if (need > ak_len) {
        seq_t *tmp = (seq_t *)realloc(ak_table, need * sizeof(seq_t));
        if (!tmp) abort();                /* malloc failure – allowed */

        /* fill any new entries using the recurrence a_k = 3*a_{k-1} - a_{k-2} */
        for (size_t i = ak_len; i < need; ++i) {
            if (i == 0)
                tmp[i] = (seq_t)1;        /* F_2 = 1 */
            else if (i == 1)
                tmp[i] = (seq_t)3;        /* F_4 = 3 */
            else
                tmp[i] = 3 * tmp[i-1] - tmp[i-2];
        }
        ak_table = tmp;
        ak_len   = need;
    }

    seq_t sum = 0;
    for (idx_t k = 0; k <= r; ++k) {
        /* compute binom(v,6) with v = n - k + 2 */
        idx_t v = n - k + 2;
        seq_t bin = (seq_t)1;
        for (idx_t i = 1; i <= 6; ++i) {
            bin = (bin * (seq_t)(v - 6 + i)) / (seq_t)i;
        }

        /* term = a_k * binom(v,6) */
        seq_t term = ak_table[(size_t)k] * bin;
        sum += term;
    }
    return sum;
}
