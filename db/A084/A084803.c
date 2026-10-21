/*
 * A084803  Least positive integers, all distinct, that satisfy Sum_{n>0}
 *          1/a(n)^z = 0, where z=(40+I*9)/41.
 * https://oeis.org/A084803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Greedy sequence definition. a(1) = 1. For n > 1, a(n) = the
 *            smallest unused positive integer m such that |S_{n-1} + 1/m^z| <
 *            |S_{n-1}|, where S_k = Σ_{i=1}^{k} 1/a(i)^z and z = (40 + I*9) /
 *            41
 * Range:     u8 n <= 1, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm maintains the partial sum with
   high‑precision long double arithmetic and scans
   integers in increasing order, keeping a bitmap of
   already used values.  Once a term is accepted it
   is stored and added to the sum.  The function
   can be called in any order; it memoises all
   terms up to the maximum n requested so far.  */

#include <math.h>
#include <stdlib.h>
#include <string.h>

static const long double RE = 40.0L / 41.0L;   /* real part of z */
static const long double IM = 9.0L  / 41.0L;   /* imaginary part of z */

static seq_t *values = NULL;          /* stored sequence terms   */
static size_t values_cap = 0;          /* allocated capacity      */
static size_t values_len = 0;          /* number of terms stored  */

static unsigned char *used = NULL;     /* bitmap of used integers */
static size_t used_cap = 0;            /* allocated size of bitmap*/

static long double sum_re = 0.0L;      /* real part of partial sum  */
static long double sum_im = 0.0L;      /* imaginary part of sum    */

static uint64_t next_candidate = 2;    /* next integer to test     */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but guard */

    /* --- initialization ------------------------------------------- */
    if (values == NULL) {
        values_cap = 16;
        values    = (seq_t *)malloc(values_cap * sizeof(seq_t));

        used_cap = 32;
        used     = (unsigned char *)calloc(used_cap, 1);

        /* a(1) = 1 */
        values[0] = (seq_t)1;
        values_len = 1;
        sum_re = 1.0L;     /* 1^( -z ) = 1 */
        sum_im = 0.0L;

        used[1] = 1;          /* mark 1 as used */
    }

    /* --- generate terms up to requested n ------------------------ */
    while (values_len < (size_t)n) {

        /* find the next admissible integer */
        for (;;) {
            /* enlarge bitmap if needed */
            if ((size_t)next_candidate >= used_cap) {
                size_t new_cap = used_cap * 2 + 32;
                unsigned char *tmp = (unsigned char *)
                    realloc(used, new_cap * sizeof(unsigned char));
                memset(tmp + used_cap, 0, (new_cap - used_cap));
                used    = tmp;
                used_cap = new_cap;
            }

            if (used[(size_t)next_candidate]) {
                ++next_candidate;
                continue;   /* already used, skip */
            }

            /* compute 1 / next_candidate^z */
            long double ln_m   = logl((long double)next_candidate);
            long double r      = expl(-RE * ln_m);
            long double angle  = -IM * ln_m;
            long double term_re = r * cosl(angle);
            long double term_im = r * sinl(angle);

            /* new partial sum */
            long double new_re = sum_re + term_re;
            long double new_im = sum_im + term_im;

            long double old_mag_sq = sum_re * sum_re + sum_im * sum_im;
            long double new_mag_sq = new_re * new_re + new_im * new_im;

            if (new_mag_sq < old_mag_sq) {   /* accepted */
                sum_re = new_re;
                sum_im = new_im;

                seq_t val = (seq_t)next_candidate;

                if (values_len >= values_cap) {
                    values_cap = values_cap * 2 + 8;
                    seq_t *tmp = (seq_t *)realloc(values,
                                    values_cap * sizeof(seq_t));
                    values = tmp;
                }
                values[values_len++] = val;
                used[(size_t)next_candidate] = 1;

                ++next_candidate;
                break;   /* proceed to next term */
            }
            ++next_candidate;  /* test next integer */
        }
    }

    /* --- return requested term ----------------------------------- */
    return values[(size_t)n - 1];
}
