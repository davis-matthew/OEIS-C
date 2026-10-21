/*
 * A089029  a(n)=n if Hofstadter's Q-sequence increases
 *          A005185(n-1)->A005185(n), a(n)=1 otherwise.
 * https://oeis.org/A089029
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if Q(n) > Q(n-1) else 1, where Q is Hofstadter's
 *            Q‑sequence * (Q(1)=1, Q(2)=1, Q(n)=Q(n−Q(n−1))+Q(n−Q(n−2)) for
 *            n>2)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE:  The sequence uses 1‑based indexing; a(2) is the first defined term. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *Q = NULL;     /* Q[0] unused, Q[1] = Q[2] = 1 */
static size_t Qlen = 0;     /* highest n already computed (1‑based) */
static size_t Qcap = 0;     /* capacity of Q array (number of elements) */

/* enlarge the Q array to hold indices up to new_cap-1 */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= Qcap)
        return;
    if (new_cap < 3)
        new_cap = 3;                 /* at least indices 0..2 needed */
    size_t new_len = new_cap * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(Q, new_len);
    if (!tmp)
        exit(1);                     /* out of memory */
    /* initialise any newly allocated elements to 0 */
    if (new_cap > Qcap) {
        for (size_t i = Qcap; i < new_cap; ++i)
            tmp[i] = 0;
    }
    Q   = tmp;
    Qcap = new_cap;
}

/* initialise the base values of Q if not already done */
static void init_Q(void)
{
    if (Qlen != 0)
        return;
    ensure_capacity(3);              /* indices 0..2 */
    Q[0] = 0;                        /* unused */
    Q[1] = 1;
    Q[2] = 1;
    Qlen = 2;
}

/* compute Q up to at least n (inclusive) */
static void compute_Q_up_to(idx_t n)
{
    init_Q();
    /* we need room for indices up to n */
    if ((size_t)n + 1 > Qcap)
        ensure_capacity((size_t)n + 1);

    while (Qlen < (size_t)n) {
        size_t i       = Qlen + 1;    /* next index to compute   */
        size_t idx1    = (size_t)Q[i - 1];          /* Q(i-1) */
        size_t idx2    = (size_t)Q[i - 2];          /* Q(i-2) */
        Q[i] = Q[i - idx1] + Q[i - idx2];
        ++Qlen;
    }
}

/* public function required by the build harness */
seq_t a(idx_t n)
{
    if (n < 2)                /* behaviour below the offset is irrelevant */
        return (seq_t)1;

    compute_Q_up_to(n);

    return (Q[n] > Q[n - 1]) ? (seq_t)n : (seq_t)1;
}
