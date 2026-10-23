/*
 * A156253  Least k such that A054353(k) >= n.
 * https://oeis.org/A156253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(a(n-1)) mod 2) + a(n-2) + 1 Initial values: a(1)=1,
 *            a(2)=2
 * Range:     u8 n <= 382, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;          /* 1‑based storage: seq[0] = a(1) */
static idx_t seq_len = 0;          /* number of values stored */
static size_t seq_cap = 0;         /* current capacity of seq array */

static void ensure_capacity(idx_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < (size_t)needed) new_cap <<= 1;
#if defined(__cplusplus)
    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
#else
    seq = realloc(seq, new_cap * sizeof(seq_t));
#endif
    seq_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used, but safe for n<1 */

    /* initialise first two terms on first call */
    if (seq_len == 0) {
        seq_cap = 16;
#if defined(__cplusplus)
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
#else
        seq = malloc(seq_cap * sizeof(seq_t));
#endif
        seq[0] = 1;                /* a(1) */
        seq[1] = 2;                /* a(2) */
        seq_len = 2;
    }

    /* return known value if within computed range */
    if (n <= seq_len) return seq[n-1];

    /* extend cache up to n */
    ensure_capacity(n);
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        /* a(i-1) is already computed */
        seq_t a_prev   = seq[i-2];
        idx_t idx = (idx_t)a_prev;          /* a(i-1) as index */

        /* a(a(i-1)) is also already computed */
        seq_t a_a_prev = seq[idx-1];
        seq_t residue  = a_a_prev & 1;       /* mod 2 */

        seq_t a_i_2 = seq[i-3];              /* a(i-2) */
        seq_t new_val = residue + a_i_2 + 1; /* recurrence */

        seq[seq_len] = new_val;
        ++seq_len;
    }

    return seq[n-1];
}
