/*
 * A056738  Positions where 2's occur in A056731.
 * https://oeis.org/A056738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n+1) = a(n) + (n+1) + #{ k | 1 ≤ k ≤ n, a(k) ≤ n }
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is monotone increasing, so the count can be maintained
   by a single pointer while the array is filled in O(n). */

#include <stdlib.h>
#include <stdint.h>

/* The following types are supplied by the build environment. */
extern seq_t a(idx_t n);

static seq_t *buf       = NULL;  /* array of computed terms, 1‑based */
static idx_t  buf_size  = 0;     /* allocated size of buf (in elements) */
static idx_t  last_used = 0;     /* highest index for which a[i] is known */
static idx_t  count_leq = 0;     /* #{k | a(k) ≤ current n} for current n */

/* Ensure the buffer can hold at least "need" elements.  The first element
   (index 0) is unused to keep indexing 1‑based. */
static void ensure_capacity(idx_t need)
{
    if (need <= buf_size) return;
    idx_t new_size = (buf_size == 0) ? need : (buf_size * 2);
    if (new_size < need) new_size = need;
    seq_t *tmp = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
    buf = tmp;
    buf_size = new_size;
}

/* Return the n-th term of A056738.  The first call initializes the
   sequence.  Calls may arrive in any order. */
seq_t a(idx_t n)
{
    if (n == 0) { /* sequence starts at offset 1; undefined for 0 */
        return 0;
    }

    /* Resize buffer to store term n */
    ensure_capacity(n + 1);

    if (n <= last_used) {
        return buf[n];
    }

    /* First time we learn about this sequence */
    if (last_used == 0) {
        buf[1] = 1;          /* a(1) = 1 */
        count_leq = 1;       /* a(1) ≤ 1 */
        last_used = 1;
    }

    /* Build terms up to n */
    for (idx_t idx = last_used + 1; idx <= n; ++idx) {
        idx_t prev_idx = idx - 1;                /* n in recurrence = idx-1 */
        seq_t cnt_seq  = (seq_t)count_leq;       /* #{k | a(k) ≤ prev_idx}  */
        buf[idx] = buf[prev_idx] + (seq_t)prev_idx + (seq_t)1 + cnt_seq;

        /* Update count_leq for next iteration (n = idx) */
        while (count_leq < idx && buf[count_leq + 1] <= (seq_t)idx) {
            ++count_leq;
        }
    }

    last_used = n;
    return buf[n];
}
