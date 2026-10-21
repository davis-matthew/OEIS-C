/*
 * A052109  a(1)=1, a(n)=a(n-a(1))+a(n-a(2))+a(n-a(3))+....a(n-a(n-1)) for
 *          n>1, with convention that a(i)=0 for i<=0.
 * https://oeis.org/A052109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A052109
 *  Recurrence:  a(1)=1,
 *               a(n)=∑_{k=1}^{n-1} a(n‑a(k))    (n>1),
 *               a(i)=0 for i≤0.
 *  The implementation memoises all values up to the requested index.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Global memoisation array */
static seq_t *seq      = NULL;   /* storage for a(0)…a(m) */
static idx_t seq_cap   = 0;      /* capacity of seq (number of elements) */
static idx_t seq_len   = 0;      /* highest index that has been computed */

/* Ensure that seq has room for at least indices 0…n */
static void
ensure_capacity(idx_t n)
{
    if (n + 1 <= seq_cap)
        return;

    idx_t new_cap = seq_cap ? seq_cap * 2 : 32;
    if (new_cap <= n + 1)
        new_cap = n + 1;

    seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_seq == NULL)
        abort();                       /* OOM – terminate */

    if (seq != NULL) {
        memcpy(new_seq, seq, seq_cap * sizeof(seq_t));
        free(seq);
    }

    /* zero‑initialise the freshly allocated portion */
    memset((void *)(new_seq + seq_cap), 0, (new_cap - seq_cap) * sizeof(seq_t));

    seq       = new_seq;
    seq_cap   = new_cap;
}

/* Return the n‑th term of A052109 (n≥1).  */
seq_t a(idx_t n)
{
    if (n == 0)   /* not used by OEIS, but harmless */
        return 0;

    /* First‑time initialisation */
    if (seq == NULL) {
        ensure_capacity(n);
        seq[0] = 0;
        seq[1] = 1;
        seq_len = 1;
    }

    /* Grow the array if required */
    ensure_capacity(n);

    /* Compute any missing terms up to n */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t sum = 0;

        for (idx_t j = 1; j < i; ++j) {
            seq_t bj = seq[j];
            if (bj >= (seq_t)i)
                break;                      /* a(n‑a(k)) = 0 for larger k */

            idx_t idx = i - (idx_t)bj;  /* i > bj, so idx ≥ 1 and < i */
            sum += seq[idx];
        }
        seq[i] = sum;
    }

    seq_len = n;
    return seq[n];
}
