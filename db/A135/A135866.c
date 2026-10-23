/*
 * A135866  G.f. A(x) satisfies A(x) = 1 + x*A(9*x)^(1/3).
 * https://oeis.org/A135866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) satisfies A(x) = 1 + x · A(9x)^(1/3); a(0) = a(1) = 1; a(n)
 *            = (1/3) · (9^(n‑1)·a(n‑1) – sum_{i+j+k=n+2} a(i)a(j)a(k))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static memoisation storage */
static seq_t *seq      = NULL;   /* array of a(n) */
static size_t seq_cap  = 0;      /* current capacity of seq   */
static seq_t *pair     = NULL;   /* convolution array: pair[s] = Σ_{i+j=s} a(i)a(j)  (i,j ≥ 1)   */
static size_t pair_cap = 0;      /* current capacity of pair  */
static idx_t  max_n    = 0;      /* largest n already computed   */
static seq_t  pow9     = 0;      /* 9^(max_n)  (max_n is the last n computed) */

/* allocate or grow an array to hold at least n+1 elements                  */
static void ensure_seq(size_t n) {
    if (n < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* allocate or grow pair array to hold indices up to n+1 (we use indices up to 2*n).  */
static void ensure_pair(size_t n) {
    size_t need = 2 * n + 2;            /* indices 0 .. 2*n+1  (safe) */
    if (need <= pair_cap) return;
    size_t new_cap = pair_cap ? pair_cap : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(pair, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    /* initialise newly allocated entries to zero */
    for (size_t i = pair_cap; i < new_cap; ++i) tmp[i] = 0;
    pair = tmp;
    pair_cap = new_cap;
}

/* initialise storage for the first two terms and their convolution pair */
static void init_storage(void) {
    if (max_n > 0) return;

    ensure_seq(1);
    seq[0] = 1;
    seq[1] = 1;
    max_n = 1;

    ensure_pair(1);
    /* initialise all indices up to pair_cap to zero */
    for (size_t i = 0; i < pair_cap; ++i) pair[i] = 0;
    /* pair[2] = a(1)*a(1) */
    pair[2] = seq[1] * seq[1];

    pow9 = 1;   /* 9^max_n = 9^1 = 9, but will be multiplied before first use */
}

seq_t a(idx_t n) {
    init_storage();

    while (max_n < n) {
        idx_t next = max_n + 1;            /* compute a(next) */

        /* update power of 9 */
        pow9 *= 9;

        /* make pair large enough for indices up to 2*next */
        ensure_pair((size_t)next);

        /* compute the convolution sum S = Σ_{i=1}^{next-1} a(i) * pair[next+2-i] */
        seq_t ssum = 0;
        for (idx_t i = 1; i < next; ++i) {
            idx_t t = next + 2 - i;        /* target pair index 3 .. next+1  */
            if (t < pair_cap)              /* missing indices contribute 0 */
                ssum += seq[i] * pair[t];
        }

        /* recurrence */
        seq_t numerator = pow9 * seq[next - 1] - ssum;
        seq_t a_next    = numerator / 3;

        /* allocate space for the new value */
        ensure_seq((size_t)next);
        seq[next] = a_next;

        /* update convolution array with contributions from a_next */
        for (idx_t k = 1; k < next; ++k) {   /* k < next  → i≠j  */
            pair[next + k] += (a_next * seq[(size_t)k]) * 2;  /* both orientations */
        }
        pair[2 * next] += a_next * a_next;     /* i = j case */

        max_n = next;
    }
    return seq[(size_t)n];
}
