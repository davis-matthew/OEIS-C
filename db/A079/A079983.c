/*
 * A079983  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={-2,1,2}.
 * https://oeis.org/A079983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079983
   Recurrence:   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-5)+4*a(n-6)-4*a(n-7)-3*a(n-8)-3*a(n-9)
                  +3*a(n-10)+3*a(n-11)-5*a(n-12)+4*a(n-13)+4*a(n-14)+a(n-15)-2*a(n-16)
                  -a(n-17)+a(n-18)-a(n-19)-a(n-20)
   Initial values:
     a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=3, a(5)=6, a(6)=12, a(7)=20,
     a(8)=35, a(9)=60, a(10)=114, a(11)=207, a(12)=375, a(13)=671,
     a(14)=1213, a(15)=2180, a(16)=3954, a(17)=7139, a(18)=12892, a(19)=23250
*/

#include <stdlib.h>

/* static array of the first 20 values */
static const seq_t base_vals[20] = {
    1, 1, 1, 1, 3, 6, 12, 20, 35, 60,
    114, 207, 375, 671, 1213, 2180, 3954, 7139, 12892, 23250
};

/* buffer for memoised values */
static seq_t *buf = NULL;          /* memory for values */
static idx_t  buf_cap = 0;         /* allocated capacity */
static idx_t  buf_computed = 0;    /* number of values already computed */

/* ensure the buffer can hold at least (n+1) entries, reallocate if needed */
static void ensure_capacity(idx_t n)
{
    if (buf_cap <= n) {
        idx_t new_cap = n + 1;
        /* Grow to next power of two for amortised efficiency */
        if (new_cap < 64) new_cap = 64;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_buf = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!new_buf) return;          /* allocation failure – caller will see garbage */
        /* zero newly allocated part */
        for (idx_t i = buf_cap; i < new_cap; ++i) new_buf[i] = 0;
        buf = new_buf;
        buf_cap = new_cap;
    }
}

/* compute sequence value a(n) */
seq_t a(idx_t n)
{
    /* initialise buffer with first 20 values only once */
    if (!buf) {
        buf_cap = 64;                         /* initial capacity */
        buf = (seq_t *)calloc(buf_cap, sizeof(seq_t));
        for (idx_t i = 0; i < 20; ++i) buf[i] = base_vals[i];
        buf_computed = 20;                    /* we have values 0..19 */
    }

    if (n < buf_computed) return buf[n];

    /* make sure we have enough room */
    ensure_capacity(n);

    /* compute missing values up to n */
    for (idx_t i = buf_computed; i <= n; ++i) {
        seq_t val = 0;
        val += buf[i - 1];
        val += buf[i - 2];
        val += buf[i - 3];
        val -= buf[i - 5];
        val += (seq_t)4 * buf[i - 6];
        val -= (seq_t)4 * buf[i - 7];
        val -= (seq_t)3 * buf[i - 8];
        val -= (seq_t)3 * buf[i - 9];
        val += (seq_t)3 * buf[i - 10];
        val += (seq_t)3 * buf[i - 11];
        val -= (seq_t)5 * buf[i - 12];
        val += (seq_t)4 * buf[i - 13];
        val += (seq_t)4 * buf[i - 14];
        val += buf[i - 15];
        val -= (seq_t)2 * buf[i - 16];
        val -= buf[i - 17];
        val += buf[i - 18];
        val -= buf[i - 19];
        val -= buf[i - 20];
        buf[i] = val;
    }

    buf_computed = n + 1;
    return buf[n];
}
