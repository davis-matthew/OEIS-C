/*
 * A087817  a(n) = a(a(a(n-1)))+a(n-a(n-1)) with a(1)=a(2)=1.
 * https://oeis.org/A087817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A087817 – a(n) = a(a(a(n-1))) + a(n-a(n-1)), with a(1)=a(2)=1
 *  Offset 1. This implementation memoises values in a static array. */

#include <stdlib.h>

/* static state for memoisation */
static seq_t *a_vals   = NULL;  /* array indexed from 1 (a_vals[0] unused) */
static idx_t  a_cap    = 0;     /* allocated capacity (max index stored)      */
static idx_t  a_last   = 0;     /* largest index that has been computed       */

/* ensure capacity for indices up to 'need' (inclusive) */
static void ensure_cap(idx_t need)
{
    if (need <= a_cap) return;
    idx_t new_cap = a_cap ? (a_cap << 1) : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = a_vals
        ? (seq_t *)realloc(a_vals, (new_cap + 1) * sizeof(seq_t))
        : (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failure */
    a_vals = tmp;
    a_cap  = new_cap;
}

/* public function computing the sequence */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* undefined for n<1 */

    /* initialise on first call */
    if (!a_vals) {
        ensure_cap(2);
        a_vals[1] = (seq_t)1;
        a_vals[2] = (seq_t)1;
        a_last   = 2;
    }

    if (n <= a_last) return a_vals[n];

    ensure_cap(n);

    for (idx_t i = a_last + 1; i <= n; ++i) {
        /* a[i-1] is already computed */
        seq_t a_prev = a_vals[i - 1];
        idx_t idx1   = (idx_t)a_prev;          /* a[i-1] */
        seq_t val1   = a_vals[idx1];           /* a[a[i-1]]       */
        idx_t idx2   = (idx_t)val1;            /* a[a[i-1]] as idx */

        seq_t val2 = a_vals[idx2];             /* a[a[a[i-1]]]    */
        idx_t idx3 = i - (idx_t)a_prev;        /* i - a[i-1]      */
        seq_t val3 = a_vals[idx3];             /* a[i - a[i-1]]   */

        a_vals[i] = val2 + val3;
    }

    a_last = n;
    return a_vals[n];
}
