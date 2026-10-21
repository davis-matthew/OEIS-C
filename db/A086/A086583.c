/*
 * A086583  Self-convolution of A086582; the first 2^n terms of this sequence
 *          gives the 2^n terms that follow the 2^n-th term of A086582.
 * https://oeis.org/A086583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0}^{n} b[k] * b[n-k] where b is A086582 defined
 *            recursively: b[0] = 1; For each power of two m, append to b the
 *            convolution of the first m elements of b with itself. The
 *            function keeps b cached for repeated calls. It works for any
 *            seq_t and idx_t types supplied by the build
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cache for the A086582 sequence */
static seq_t *b_buf  = NULL;   /* b[0 .. b_len-1] */
static idx_t  b_len  = 0;      /* number of elements stored */

/* Temporary buffer for convolution */
static seq_t *tmp_buf = NULL;
static size_t tmp_cap = 0;

seq_t a(idx_t n)
{
    /* Find smallest power of two strictly greater than n. */
    idx_t L = 1;
    while (L <= n) L <<= 1;

    /* Ensure capacity for b_buf */
    if (b_len < L) {
        seq_t *newb = (seq_t *)realloc(b_buf, L * sizeof(seq_t));
        if (!newb) return 0;            /* allocation failure */
        b_buf = newb;
    }

    /* Initialise first element if buffer is empty. */
    if (b_len == 0) {
        b_buf[0] = (seq_t)1;
        b_len   = 1;
    }

    /* Ensure capacity for tmp_buf */
    if (tmp_cap < (size_t)L) {
        seq_t *newtmp = (seq_t *)realloc(tmp_buf, L * sizeof(seq_t));
        if (!newtmp) return 0;
        tmp_buf = newtmp;
        tmp_cap = L;
    }

    /* Grow b_buf in powers of two until it reaches length L. */
    idx_t cur = b_len;
    while (cur < L) {
        /* Compute convolution of first cur elements. */
        for (idx_t k = 0; k < cur; ++k) {
            seq_t sum = 0;
            for (idx_t i = 0; i <= k; ++i)
                sum += b_buf[i] * b_buf[k - i];
            tmp_buf[k] = sum;
        }

        /* Append the computed block to b_buf. */
        for (idx_t i = 0; i < cur; ++i)
            b_buf[cur + i] = tmp_buf[i];

        cur <<= 1;
    }
    b_len = L;   /* length of populated part (≥ L) */

    /* Compute requested a(n). */
    seq_t res = 0;
    for (idx_t i = 0; i <= n; ++i)
        res += b_buf[i] * b_buf[n - i];

    return res;
}
