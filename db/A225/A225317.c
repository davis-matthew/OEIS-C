/*
 * A225317  Numbers k such that 10k is a partition number.
 * https://oeis.org/A225317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 17, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225317:  numbers k such that 10k is a partition number.
 * The code generates ordinary partition numbers via Euler's pentagonal
 * recurrence.  Whenever a partition number is divisible by 10, the
 * quotient k=p(n)/10 is stored.  a(n) is the n‑th stored k.
 */

#include <stdlib.h>

/* internal dynamic arrays for partition numbers and the sequence terms */
static seq_t *p_arr = NULL;  /* partitions p[0], p[1], ... ,  p(p_len-1) */
static size_t   p_cap = 0;
static size_t   p_len = 0;

static seq_t *k_arr = NULL;  /* sequence a(1), a(2), ... */
static size_t   k_cap = 0;
static size_t   k_len = 0;

/* grow the partition array to at least new_cap elements */
static void grow_p_arr(size_t new_cap)
{
    if (new_cap <= p_cap) return;
    size_t nc = p_cap ? p_cap : 16;
    while (nc < new_cap) nc <<= 1;
    seq_t *tmp = (seq_t *)realloc(p_arr, sizeof(seq_t) * nc);
    p_arr = tmp;
    p_cap = nc;
}

/* grow the sequence array to at least new_cap elements */
static void grow_k_arr(size_t new_cap)
{
    if (new_cap <= k_cap) return;
    size_t nc = k_cap ? k_cap : 16;
    while (nc < new_cap) nc <<= 1;
    seq_t *tmp = (seq_t *)realloc(k_arr, sizeof(seq_t) * nc);
    k_arr = tmp;
    k_cap = nc;
}

/* compute the next partition number p[p_len] and, if divisible by 10,
 * store k = p/10 into the sequence array.  `p_len` is the number of
 * partition values already stored.  After this call, p_len and k_len
 * are updated accordingly. */
static void compute_next_partition(void)
{
    /* initialise the base partition p(0)=1 if needed */
    if (p_len == 0) {
        grow_p_arr(1);
        p_arr[0] = (seq_t)1;
        p_len = 1;
    }

    idx_t i = (idx_t)p_len;          /* index of the new partition */
    seq_t val = 0;                   /* accumulator for Euler's sum */

    for (idx_t k = 1;; ++k) {
        idx_t g1 = k * ((idx_t)3 * k - 1) / 2;
        if (g1 > i) break;           /* no more generalized pentagonals */

        seq_t term = p_arr[(size_t)(i - g1)];
        if (k & 1) val += term;
        else      val -= term;

        idx_t g2 = k * ((idx_t)3 * k + 1) / 2;
        if (g2 > i) continue;
        term = p_arr[(size_t)(i - g2)];
        if (k & 1) val += term;
        else      val -= term;
    }

    /* store the new partition */
    grow_p_arr(p_len + 1);
    p_arr[(size_t)i] = val;
    ++p_len;

    /* if divisible by 10, record k = p/10 */
    if ((val % (seq_t)10) == 0) {
        seq_t kval = val / (seq_t)10;
        grow_k_arr(k_len + 1);
        k_arr[(size_t)k_len++] = kval;
    }
}

/* required public routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety guard – offset is 1 */

    /* generate partitions until we have the requested number of terms */
    while (k_len < (size_t)n) compute_next_partition();

    return k_arr[(size_t)n - 1];
}
