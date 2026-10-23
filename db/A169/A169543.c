/*
 * A169543  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * 1176*a(n) - 48*a(n-1) -
 *            48*a(n-2) - … - 48*a(n-33) + a(n-34) = 0 * for n ≥ 35, * with
 *            initial values a(0)=1, a(1)=50, …, a(34) computed from the
 *            in‑homogeneous * recurrence derived from the generating
 *            function. * * The implementation keeps the sequence in a...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq = NULL;          /* full sequence storage             */
static size_t  seq_cap = 0;        /* capacity of seq                   */
static size_t  seq_len = 0;        /* number of terms stored           */
static seq_t   ring[34];           /* circular buffer of last 34 values */
static seq_t   sum33 = 0;          /* sum of the last 33 values         */
static int     initialized = 0;     /* flag whether initial terms set   */

static void
ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (seq_cap < need) {
        size_t newcap = need > 35 ? need : 35;
        seq_t *newseq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        if (!newseq) { abort(); }
        seq = newseq;
        seq_cap = newcap;
    }
}

static void
initialize(void)
{
    /* first compute a(0)..a(34) using the inhomogeneous recurrence
       derived from the generating function.                       */
    ensure_capacity(34);
    seq[0] = 1;
    seq_len = 1;

    seq_t cum = 1;                /* cumulative sum a(0)..a(m-1) */

    for (idx_t m = 1; m <= 33; ++m) {
        seq[m] = (seq_t)2 + (seq_t)48 * cum;
        seq_len++;
        cum += seq[m];
    }
    /* a(34) has the extra -1224*a(0) term                      */
    seq[34] = (seq_t)1 + (seq_t)48 * cum - (seq_t)1224 * seq[0];
    cum += seq[34];
    seq_len = 35;

    /* initialise the ring buffer with the first 34 values      */
    for (int i = 0; i < 34; ++i) ring[i] = seq[i];

    /* sum of a(2)..a(34) for the first use of the recurrence  */
    sum33 = 0;
    for (idx_t i = 2; i <= 34; ++i) sum33 += seq[i];

    initialized = 1;
}

seq_t
a(idx_t n)
{
    if (!initialized) initialize();

    ensure_capacity(n);

    /* if we haven't yet computed up to n, compute further */
    if (seq_len <= (size_t)n) {
        for (idx_t m = seq_len; m <= n; ++m) {
            /* recurrence: a(m) = (48*sum33 - a(m-34)) / 1176      */
            seq_t nom = (seq_t)48 * sum33 - ring[(m - 34) % 34];
            seq_t val = nom / 1176;     /* division is exact          */

            /* ring buffer bookkeeping                               */
            idx_t idx_new = m % 34;          /* where to store a(m)          */
            idx_t idx_old = (m - 33) % 34;    /* value to be removed from sum */

            seq_t old_val = ring[idx_old];

            ring[idx_new] = val;
            /* update sum of the last 33 values */
            sum33 += val;
            sum33 -= old_val;

            /* store full value for future direct access           */
            seq[m] = val;
        }
        seq_len = (size_t)n + 1;
    }

    return seq[n];
}
