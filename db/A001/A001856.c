/*
 * A001856  A self-generating sequence: every positive integer occurs as
 *          a(i)-a(j) for a unique pair i,j.
 * https://oeis.org/A001856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1)=1, a(2)=2, a(2n+1) = 2*a(2n), a(2n+2) = a(2n+1) + r(n),
 *            where r(n) = smallest positive number not of form a(j)-a(i),
 *            1<=i<j<=2n+1. Small differences are tracked incrementally in a
 *            bitmap
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define A1856_MAXN 20010
#define A1856_SHIFT 21
#define A1856_SEEN (1UL << A1856_SHIFT)

static seq_t a1856_terms[A1856_MAXN + 1];
static unsigned char a1856_seen[A1856_SEEN];
static idx_t a1856_have = 0;
static size_t a1856_rptr = 1;

static void a1856_add(idx_t k) {
    idx_t i;
    for (i = k - 1; i >= 1; i--) {
        seq_t d = a1856_terms[k] - a1856_terms[i];
        if ((d >> A1856_SHIFT) != 0) break;
        a1856_seen[(size_t)(long long)d] = 1;
    }
}

seq_t a(idx_t n) {
    if (n < 1 || n > A1856_MAXN) return 0;
    if (a1856_have == 0) {
        a1856_terms[1] = 1;
        a1856_terms[2] = 2;
        a1856_add(2);
        a1856_have = 2;
    }
    while (a1856_have < n) {
        idx_t k = a1856_have + 1;
        if (k % 2 == 1) {
            a1856_terms[k] = a1856_terms[k - 1] * 2;
        } else {
            while (a1856_rptr < A1856_SEEN - 1 && a1856_seen[a1856_rptr]) a1856_rptr++;
            a1856_terms[k] = a1856_terms[k - 1] + (seq_t)(unsigned long long)a1856_rptr;
        }
        a1856_add(k);
        a1856_have = k;
    }
    return a1856_terms[n];
}
