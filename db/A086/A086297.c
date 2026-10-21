/*
 * A086297  Squarefree kernels of 7-smooth numbers.
 * https://oeis.org/A086297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 1089, u32 n <= 8426, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = squarefree kernel of the nth 7‑smooth number.
 * That is, let s = the smoothed number A002473(n); then
 * a(n) = product of the distinct prime factors 2, 3, 5, 7 of s.
 */

#include <stdlib.h>

/* =========================================================================
 *  The routine maintains a static array large enough for the required
 *  range (up to about 10 000 elements).  All indices are zero‑based
 *  internally, but the public interface uses the OEIS offset 1.
 *  The array elements are of type seq_t, which can be an arbitrary‑
 *  precision integer; the standard allocation functions are never
 *  used, so the code compiles as C11 and C++17 alike.
 *  =========================================================================
 */
#define MAX_SMOOTH 20000          /* > required 10 000 */

static seq_t  smooth[MAX_SMOOTH];
static idx_t   smooth_len;        /* number of elements actually in use */
static idx_t   i2, i3, i5, i7;    /* pointers for generating the next smooth number */

/* --------------------------------------------------------------------- */
/* initialise the static tables the first time any call to a() is made    */
static void init_smooth(void)
{
    if (smooth_len != 0) return;          /* already initialised */
    smooth_len = 1;
    smooth[0] = (seq_t)1;                 /* the 0‑th 7‑smooth number (which corresponds to a(1)) */
    i2 = i3 = i5 = i7 = 0;
}

/* --------------------------------------------------------------------- */
/* generate smooth numbers until at least n entries exist                */
static void ensure_smooth(idx_t n)
{
    if (smooth_len >= n) return;          /* nothing to do */

    while (smooth_len < n) {
        seq_t n2 = smooth[i2] * (seq_t)2;
        seq_t n3 = smooth[i3] * (seq_t)3;
        seq_t n5 = smooth[i5] * (seq_t)5;
        seq_t n7 = smooth[i7] * (seq_t)7;

        seq_t next = n2;
        if (n3 < next) next = n3;
        if (n5 < next) next = n5;
        if (n7 < next) next = n7;

        if (smooth_len < MAX_SMOOTH)      /* safety guard */
            smooth[smooth_len++] = next;
        else
            return;                       /* array exhausted – should never happen */

        if (next == n2) i2++;
        if (next == n3) i3++;
        if (next == n5) i5++;
        if (next == n7) i7++;
    }
}

/* --------------------------------------------------------------------- */
/* compute the squarefree kernel of x (product of its distinct prime factors) */
static seq_t rad(const seq_t x)
{
    seq_t r = (seq_t)1;
    if ((x % (seq_t)2) == (seq_t)0) r *= (seq_t)2;
    if ((x % (seq_t)3) == (seq_t)0) r *= (seq_t)3;
    if ((x % (seq_t)5) == (seq_t)0) r *= (seq_t)5;
    if ((x % (seq_t)7) == (seq_t)0) r *= (seq_t)7;
    return r;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1 – harmless fallback      */
    init_smooth();                  /* ensure that the tables are ready */
    ensure_smooth(n);               /* generate up to the required index      */
    seq_t s = smooth[n - 1];        /* convert OEIS offset 1 to zero‑based    */
    return rad(s);
}
