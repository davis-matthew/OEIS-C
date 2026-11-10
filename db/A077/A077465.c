/*
 * A077465  Values of n such that A006046(n)/n^theta, where
 *          theta=log(3)/log(2), is a local minimum, computed according to
 *          Harborth's recurrence.
 * https://oeis.org/A077465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3; a(n+1) = 2*a(n)+s, where s = +1 or -1 is the
 *            choice that gives the smaller A006046(m)/m^theta,
 *            theta=log(3)/log(2) (Harborth's recurrence). A006046(0)=0,
 *            A006046(1)=1, A006046(2k)=3*A006046(k),
 *            A006046(2k+1)=2*A006046(k)+A006046(k+1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"

static seq_t *H_terms = NULL;
static size_t H_cnt = 0;
static size_t H_cap = 0;
static long double H_lo, H_mid, H_hi, H_m;

static void H_push(seq_t v) {
    if (H_cnt >= H_cap) {
        size_t nc = H_cap ? H_cap * 2 : 64;
        if (H_terms == NULL) {
            H_terms = mem_new(seq_t, nc);
        } else {
            H_terms = mem_resize(seq_t, H_terms, nc);
        }
        H_cap = nc;
    }
    H_terms[H_cnt] = v;
    H_cnt++;
}

static void H_extend(void) {
    if (H_cnt == 0) {
        H_push((seq_t)0);   /* unused slot 0 */
        H_push((seq_t)1);   /* a(1) */
        H_push((seq_t)3);   /* a(2) */
        H_lo = 3.0L; H_mid = 5.0L; H_hi = 9.0L; H_m = 3.0L; /* A006046(2..4) for m=3 */
        return;
    }
    long double theta = logl(3.0L) / logl(2.0L);
    long double c = 2.0L * H_m;
    long double Ap = 2.0L * H_mid + H_hi;   /* A006046(c+1) */
    long double Am = 2.0L * H_lo + H_mid;   /* A006046(c-1) */
    long double diff = log1pl((Ap - Am) / Am) - theta * log1pl(2.0L / (c - 1.0L));
    seq_t prev = H_terms[H_cnt - 1];
    if (diff < 0.0L) {
        H_push(2 * prev + 1);
        long double nl = 3.0L * H_mid, nm = Ap, nh = 3.0L * H_hi;
        H_lo = nl; H_mid = nm; H_hi = nh;
        H_m = c + 1.0L;
    } else {
        H_push(2 * prev - 1);
        long double nl = 3.0L * H_lo, nm = Am, nh = 3.0L * H_mid;
        H_lo = nl; H_mid = nm; H_hi = nh;
        H_m = c - 1.0L;
    }
}

seq_t a(idx_t n) {
    size_t k = (size_t)n;
    while (H_cnt <= k) H_extend();
    return H_terms[k];
}
