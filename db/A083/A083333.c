/*
 * A083333  a(n) = 10*a(n-2) - 16*a(n-4) for n>=4, with a(0)=a(1)=1, a(2)=6,
 *          a(3)=10.
 * https://oeis.org/A083333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-2) - 16*a(n-4) for n≥4 a(0)=1, a(1)=1, a(2)=6,
 *            a(3)=10
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for memoisation */
static seq_t *seq = NULL;
static size_t sz = 0;   /* number of terms currently computed */
static size_t cap = 0;  /* capacity of the array */

/* ensure the array can hold indices up to 'need' (exclusive) */
static void ensure_size(size_t need)
{
    if (need <= cap)
        return;
    size_t new_cap = cap ? (cap << 1) : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* sequence value */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!seq) {
        cap = 8;
        seq = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)6;
        seq[3] = (seq_t)10;
        sz = 4;
    }

    size_t idx = (size_t)n;
    if (idx >= sz) {
        size_t need = idx + 1;
        ensure_size(need);
        for (size_t i = sz; i <= idx; ++i) {
            if (i < 4)
                continue;                                           /* initial terms already set */
            seq[i] = (seq_t)10 * seq[i - 2] - (seq_t)16 * seq[i - 4];
        }
        sz = need;
    }
    return seq[idx];
}
