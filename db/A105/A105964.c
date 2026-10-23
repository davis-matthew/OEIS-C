/*
 * A105964  Expansion of x*(1+x^3-x^6+x^7)/(1-x^6)^2.
 * https://oeis.org/A105964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-6) - a(n-12) for n > 11; initial values: a(0)=0,
 *            a(1)=1, a(2)=0, a(3)=0, a(4)=1, a(5)=0, a(6)=0, a(7)=1, a(8)=1,
 *            a(9)=0, a(10)=2, a(11)=0
 * Range:     u8 n <= 979, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  Cached values of the sequence  */
static seq_t *seq_vals = NULL;
static idx_t  seq_len  = 0;   /* number of stored entries */

/*  Initialise the cache with the 12 known base terms  */
static void init_seq(void)
{
    if (seq_vals != NULL)
        return;               /* already initialised */

    seq_len = 12;             /* indices 0..11 inclusive   */
    seq_vals = (seq_t *)malloc(seq_len * sizeof(seq_t));
    if (!seq_vals)
        return;               /* allocation failed – callers get 0  */

    seq_vals[0]  = 0;
    seq_vals[1]  = 1;
    seq_vals[2]  = 0;
    seq_vals[3]  = 0;
    seq_vals[4]  = 1;
    seq_vals[5]  = 0;
    seq_vals[6]  = 0;
    seq_vals[7]  = 1;
    seq_vals[8]  = 1;
    seq_vals[9]  = 0;
    seq_vals[10] = 2;
    seq_vals[11] = 0;
}

/*  Ensure that seq_vals[n] is computed and stored  */
static void ensure_seq(idx_t n)
{
    if (seq_vals == NULL)
        init_seq();

    if (n < seq_len)
        return;                                      /* already available */

    /* Resize to accommodate indices up to n  */
    idx_t old_len = seq_len;
    seq_len = n + 1;
    seq_vals = (seq_t *)realloc(seq_vals, seq_len * sizeof(seq_t));
    if (!seq_vals)
        return;               /* allocation failed – caller will get 0  */

    /*  Compute new entries using the recurrence  */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i < 12) {
            /* base terms are already set by init_seq  */
            continue;
        }
        /* a(i) = 2*a(i-6) - a(i-12) */
        seq_t tmp = seq_vals[i - 6];
        tmp += tmp;                     /* tmp = 2*a(i-6)        */
        if (tmp < seq_vals[i - 12]) {   /* underflow – should not happen  */
            /* protect against underflow; the definition guarantees non‑negative result */
            seq_vals[i] = 0;
        } else {
            seq_vals[i] = tmp - seq_vals[i - 12];
        }
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* simple guard for first call */

    ensure_seq(n);
    return seq_vals[n];
}
