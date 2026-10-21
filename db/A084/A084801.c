/*
 * A084801  Least positive integers, all distinct, that satisfy Sum_{n>0}
 *          1/a(n)^z = 0, where z=(12+I*5)/13.
 * https://oeis.org/A084801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by greedily choosing the smallest unused
 *            positive integer a such that adding 1/a^z to the current partial
 *            sum strictly decreases its magnitude, with z = (12 + 5i)/13
 * Range:     u8 n <= 52, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stddef.h>

static const long double RE = 12.0L / 13.0L;   /* real part of z */
static const long double IM = 5.0L  / 13.0L;   /* imaginary part of z */

#define USED_SIZE 1048576                    /* enough for all required a(n) */
static unsigned char used_arr[USED_SIZE];   /* 0 = unused, 1 = used */

/* Table of already computed a(n) values.  200 000 entries are plenty for n≤10000. */
#define MAXSEQ 200000
static seq_t seq_buf[MAXSEQ];
static size_t seq_len = 0;                  /* number of terms stored */

/* current complex sum S = sum_{k=1}^{seq_len} 1/a(k)^z */
static long double sum_real = 0.0L, sum_imag = 0.0L;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but harmless */

    /* initialization of the first term */
    if (seq_len == 0) {
        seq_buf[0] = (seq_t)1;
        seq_len = 1;
        sum_real = 1.0L;
        sum_imag = 0.0L;
        if (1 < USED_SIZE) used_arr[1] = 1;
    }

    /* compute further terms until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* look for the smallest unused candidate that
           decreases the magnitude of the partial sum */
        unsigned int cand = 2;
        long double old_mag2 = sum_real * sum_real + sum_imag * sum_imag;

        for (;;) {
            if (cand < USED_SIZE && used_arr[cand]) {
                ++cand;
                continue;
            }

            /* compute 1/cand^z = cand^(-RE) * exp(-i*IM*ln(cand)) */
            long double ln      = logl((long double)cand);
            long double mag     = expl(-RE * ln);
            long double theta   = IM * ln;
            long double cosv    = cosl(theta);
            long double sinv    = sinl(theta);
            long double t_real  = mag * cosv;
            long double t_imag  = -mag * sinv;

            long double new_real = sum_real + t_real;
            long double new_imag = sum_imag + t_imag;
            long double new_mag2 = new_real * new_real + new_imag * new_imag;

            /* strict decrease (use a small epsilon for safety) */
            if (new_mag2 <= old_mag2 - 1e-18L) {
                /* accept this candidate */
                if (cand < USED_SIZE) used_arr[cand] = 1;
                seq_buf[seq_len++]       = (seq_t)cand;
                sum_real                 = new_real;
                sum_imag                 = new_imag;
                break;
            }
            ++cand;
        }
    }

    return seq_buf[(size_t)n - 1];
}
