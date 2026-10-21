/*
 * A226896  Position of log n in the joint ranking of harmonic numbers H(k)
 *          and {log k}, for k >= 1; complement of A226894.
 * https://oeis.org/A226896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + #{ k ≥ 1 : H(k) < log n } (H(k) = harmonic number)
 * Range:     u8 n <= 164, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* harmonic numbers H(k) stored as doubles: H[0] = H(1), H[1] = H(2), ... */
static double *Harmonic = NULL;
static size_t HarmonicCnt = 0;   /* number of elements stored */
static size_t HarmonicCap = 0;   /* allocated capacity */

/* ensure Harmonic has at least cap elements allocated */
static void ensure_cap(size_t cap) {
    if (cap <= HarmonicCap) return;
    size_t newcap = HarmonicCap ? HarmonicCap * 2 : 64;
    if (newcap < cap) newcap = cap;
    double *newptr = (double *)realloc(Harmonic, newcap * sizeof(double));
    if (!newptr) {
        /* allocation failed: abort */
        abort();
    }
    Harmonic = newptr;
    HarmonicCap = newcap;
}

/* append new harmonic value H = old + 1/(next_k) */
static void append_harmonic(double H) {
    ensure_cap(HarmonicCnt + 1);
    Harmonic[HarmonicCnt++] = H;
}

/* sequence a(n) */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                 /* undefined for n=0, just return 0 */

    /* compute log(n) as double */
    double ln_n = log((double)n);

    /* extend harmonic array until last harmonic > ln_n */
    if (HarmonicCnt == 0 || Harmonic[HarmonicCnt-1] <= ln_n) {
        double hsum = HarmonicCnt ? Harmonic[HarmonicCnt-1] : 0.0;
        while (hsum <= ln_n) {
            size_t nextk = HarmonicCnt + 1;   /* next k = current count + 1 (k starting at 1) */
            hsum += 1.0 / (double)nextk;
            append_harmonic(hsum);
        }
    }

    /* number of harmonic terms < log n is HarmonicCnt - 1 */
    idx_t cntH = (idx_t)(HarmonicCnt - 1);

    return (seq_t)n + (seq_t)cntH;
}
