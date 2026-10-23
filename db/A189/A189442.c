/*
 * A189442  a(n) = A140230(n) / A016116(n-1).
 * https://oeis.org/A189442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-4) - a(n-8) for n>=9 Initial values:
 *            a(1)=1,a(2)=3,a(3)=1,a(4)=-3, a(5)=-5,a(6)=-7,a(7)=-1,a(8)=7
 * Range:     i8 n <= 64, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t* seq_cache = NULL;
static idx_t  seq_len   = 0;

static const seq_t init_vals[8] = {
    (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)-3,
    (seq_t)-5, (seq_t)-7, (seq_t)-1, (seq_t)7
};

static void ensure_seq_cache(idx_t n)
{
    if (seq_len >= n) return;
    idx_t new_len = (seq_len == 0) ? ((n > 16) ? n : 16) : seq_len * 2;
    if (new_len < n) new_len = n;
    while (new_len < n) new_len <<= 1;
    /* cast is required for C++ compatibility */
    seq_t* tmp = (seq_t*)realloc(seq_cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    for (idx_t i = seq_len; i < new_len; ++i) {
        idx_t k = i + 1;
        if (k <= 8) {
            seq_cache[i] = init_vals[k-1];
        } else {
            seq_cache[i] = - ((seq_t)2 * seq_cache[i-4] + seq_cache[i-8]);
        }
    }
    seq_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_seq_cache(n);
    return seq_cache[n-1];
}
