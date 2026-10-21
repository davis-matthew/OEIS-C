/*
 * A084812  a(1)=1; for n >= 2, choose a(n) so that |Sum_{k=1..n} 1/a(k)^z| <
 *          |Sum_{k=1..n-1} 1/a(k)^z|, where z = 1/2 + i*2*Pi.
 * https://oeis.org/A084812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084812: a(1)=1. For n≥2, a(n) is the smallest positive integer not
   used earlier that makes |S_{n‑1} + a(n)^(−z)| < |S_{n‑1}|, where
   z = 1/2 + i·2·π.  The partial sums S_k are stored as the real and
   imaginary parts of a long double; the inequality is tested by
   comparing squared magnitudes with a small tolerance. */
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* static state for memoisation */
static seq_t *seq = NULL;          /* array of computed a(n) (1‑based)   */
static size_t seq_cap = 0;         /* current allocation size           */
static idx_t seq_len = 0;          /* number of terms computed          */

static long double S_real = 0.0L;  /* real part of current partial sum */
static long double S_im   = 0.0L;  /* imag part of current partial sum */
static long double old_sq = 0.0L;  /* |S|^2 of previous partial sum   */
static idx_t last_used = 0;        /* last a(n) selected (initially 0) */

#define EPS 1e-18L

/* ensure capacity for at least n+1 entries (1‑based index used) */
static void ensure_capacity(idx_t n)
{
    if (seq_cap > (size_t)n) return;
    size_t new_cap = (seq_cap > 0) ? seq_cap << 1 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq = (seq_t*)realloc(seq, sizeof(seq_t)*(new_cap+1));
    seq_cap = new_cap;
}

/* main function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but keep silent */

    if (seq_len == 0) {    /* first call – initialise */
        ensure_capacity(16);
        seq[1] = (seq_t)1;
        seq_len = 1;
        last_used = 1;
        S_real = 1.0L; S_im = 0.0L;
        old_sq = 1.0L;               /* |1|^2 */
    }

    if (n <= seq_len) return seq[n];

    /* constants for the complex exponent */
    const long double PI   = acos(-1.0L);
    const long double TWOPI = 2.0L * PI;

    /* generate terms up to n */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        idx_t cand = last_used + 1;

        /* search for first candidate that decreases the magnitude */
        while (true) {
            long double pre  = sqrtl((long double)cand);
            long double tmag = 1.0L / pre;
            long double ang  = TWOPI * logl((long double)cand);
            long double cosa = cosl(ang);
            long double sina = sinl(ang);

            /* new partial sum components */
            long double new_re = S_real + tmag * cosa;
            long double new_im = S_im   - tmag * sina;
            long double new_sq = new_re * new_re + new_im * new_im;

            if (new_sq < old_sq - EPS) {
                /* this candidate works */
                seq[i]   = (seq_t)cand;
                S_real   = new_re;
                S_im     = new_im;
                old_sq   = new_sq;
                last_used = cand;
                break;
            }
            cand++;
        }
    }

    seq_len = n;
    return seq[n];
}
