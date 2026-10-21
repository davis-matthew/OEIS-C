/*
 * A093087  "Lucas in digits": start with a(1)=1, a(2)=3; repeatedly adjoin
 *          digits of sum of next two terms.
 * https://oeis.org/A093087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas in digits: a(1)=1, a(2)=3; for each consecutive pair
 * a(k)+a(k+1) write its decimal digits into the sequence one by
 * one.  When all digits of a sum have been written the next pair
 * (k+1,k+2) is used.  This produces the digit sequence
 * 1,3,4,7,1,1,8,2,9,1,0,1,1,1,0,1,1,2,2,1,1,2,3,4,…            */

#include <stdlib.h>

/* static state */
static seq_t *seq     = NULL;   /* digits array (1‑based logic but 0‑based indexing) */
static size_t  cap    = 0;      /* allocated capacity */
static idx_t   sz     = 0;      /* number of digits stored */
static idx_t   sumIdx = 1;      /* current pair index (1‑based) for the sum being processed */
static int     buf[2];          /* digits of the current sum (max 2 digits) */
static int     buflen = 0;      /* length of current buffer */
static int     bufpos = 0;      /* next digit to output from buffer */
static int     init   = 0;      /* initialization flag */

/* forward declarations */
static void init_sequence(void);
static void ensure_capacity(idx_t needed);
static void extend_to(idx_t n);

/* initialise first two terms and the first sum buffer */
static void init_sequence(void)
{
    if (init) return;
    cap = 2;
    seq = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!seq) abort();
    seq[0] = 1;                    /* a(1) */
    seq[1] = 3;                    /* a(2) */
    sz  = 2;

    /* compute digits of a(1)+a(2) = 4 */
    int s = (int)seq[0] + (int)seq[1];
    if (s >= 10) {
        buf[0]   = s / 10;
        buf[1]   = s % 10;
        buflen   = 2;
    } else {
        buf[0]   = s;
        buflen   = 1;
    }
    bufpos = 0;
    sumIdx = 1;
    init   = 1;
}

/* ensure the array has enough capacity for at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if ((size_t)needed <= cap) return;
    size_t ncap = cap > 0 ? cap : 2;
    while (ncap < (size_t)needed) ncap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, ncap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    cap = ncap;
}

/* generate sequence up to index n (1‑based) */
static void extend_to(idx_t n)
{
    init_sequence();
    if (n <= sz) return;
    ensure_capacity(n);
    while (sz < n) {
        if (bufpos >= buflen) {               /* current sum exhausted – generate next sum */
            ++sumIdx;                         /* next pair starts at sumIdx (1‑based) */
            int sum = (int)seq[sumIdx-1] + (int)seq[sumIdx];
            if (sum >= 10) {
                buf[0]   = sum / 10;
                buf[1]   = sum % 10;
                buflen   = 2;
            } else {
                buf[0]   = sum;
                buflen   = 1;
            }
            bufpos = 0;
        }
        seq[sz++] = (seq_t)buf[bufpos++];
    }
}

/* required external function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety: sequence is 1‑based */
    extend_to(n);
    return seq[n-1];
}
