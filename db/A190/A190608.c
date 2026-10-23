/*
 * A190608  a(1)=a(2)=1; thereafter a(n) = 2*(a(ceiling(n/2))-a(floor(n/2))).
 * https://oeis.org/A190608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=a(2)=1, a(n)=2*(a(ceil(n/2))-a(floor(n/2))) for n>=3
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
static seq_t *seq_cache = NULL;
static idx_t seq_allocated = 0;
static idx_t seq_computed = 0;

static void seq_ensure_alloc(idx_t n)
{
    /* allocate at least n+1 entries (index starts at 1) */
    if (n + 1 <= seq_allocated)
        return;
    idx_t new_alloc = n + 1;
    if (seq_allocated > 0 && new_alloc < seq_allocated * 2)
        new_alloc = seq_allocated * 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, (size_t)new_alloc * sizeof(seq_t));
    if (!tmp)
        abort(); /* out of memory */
    seq_cache = tmp;
    seq_allocated = new_alloc;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined but harmless */
    seq_ensure_alloc(n);
    while (seq_computed < n) {
        ++seq_computed;
        if (seq_computed <= 2) {
            seq_cache[seq_computed] = (seq_t)1;
        } else {
            idx_t left  = seq_computed / 2;           /* floor(n/2) */
            idx_t right = (seq_computed + 1) / 2;     /* ceil(n/2)  */
            seq_t diff = seq_cache[right] - seq_cache[left];
            seq_cache[seq_computed] = (seq_t)2 * diff;
        }
    }
    return seq_cache[n];
}
