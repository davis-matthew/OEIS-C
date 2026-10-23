/*
 * A026469  a(1) = 1; for n > 1, a(n) = least positive integer > a(n-1) and
 *          not equal to a(i)^2 + a(j)^2 for 1<=i<=j<=n-1.
 * https://oeis.org/A026469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026469: a(1)=1; for n>1, a(n)=least integer > a(n-1)
   not representable as a(i)^2 + a(j)^2 for 1≤i≤j≤n-1.
   The implementation builds the terms on demand, caching all
   previously computed values. */
#include <stdlib.h>
#include <stdbool.h>

/* Cached sequence values.  They are stored in the order of
   increasing index; seq_len holds the number of elements
   currently stored. */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

/* An array holding the squares of the terms in the same order.
   Since seq is strictly increasing, this array is also sorted. */
static seq_t *sq       = NULL;
static size_t sq_len   = 0;
static size_t sq_cap   = 0;

/* Ensure a capacity of at least 'needed'.  The array is grown
   by repeated doubling.  The cast to (seq_t*) allows compilation
   in both C11 and C++17. */
static void grow_seq(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq     = tmp;
    seq_cap = new_cap;
}

static void grow_sq(size_t needed)
{
    if (needed <= sq_cap) return;
    size_t new_cap = sq_cap ? sq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(sq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    sq     = tmp;
    sq_cap = new_cap;
}

/* Test whether `cand` can be expressed as the sum of two squares
   of already stored terms.  Because `sq` contains the squares in
   ascending order, a two‑pointer scan yields linear time. */
static bool is_forbidden(seq_t cand, size_t cnt)
{
    if (cnt == 0) return false;
    size_t l = 0, r = cnt - 1;
    while (l <= r) {
        seq_t sum = sq[l] + sq[r];
        if (sum == cand) return true;
        if (sum < cand) ++l;
        else --r;
    }
    return false;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined; return 0 for safety */
    /* Ensure the cache contains all terms up to the required index. */
    while (seq_len < (size_t)n) {
        seq_t cand = (seq_len == 0) ? (seq_t)1 : seq[seq_len - 1] + (seq_t)1;
        while (is_forbidden(cand, seq_len)) ++cand;
        grow_seq(seq_len + 1);
        grow_sq(seq_len + 1);
        seq[seq_len] = cand;
        sq[seq_len]  = cand * cand;
        ++seq_len;
    }
    return seq[n - 1];
}
