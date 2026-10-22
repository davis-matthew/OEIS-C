/*
 * A005847  Imaginary quadratic fields with class number 2 (a finite
 *          sequence).
 * https://oeis.org/A005847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   search over squarefree n; let D = -n if n == 3 (mod 4), else D =
 *            -4n (the field discriminant of Q(sqrt(-n))); keep n when the
 *            class number h(D), counted as the number of reduced forms
 *            (a,b,c) with b^2 - 4ac = D, is 2. The sequence is finite (18
 *            terms, largest 427), so the search stops at 2000
 * Range:     u8 n <= 15, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   class_number_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/class_number_ll.h"

#define A005847_LIMIT 2000

static bool squarefree_ll(long long n) {
    for (long long p = 2; p * p <= n; p++)
        if (n % (p * p) == 0) return false;
    return true;
}

static long long terms[64];
static int nterms = -1;

static void build_terms(void) {
    nterms = 0;
    for (long long n = 1; n <= A005847_LIMIT && nterms < 64; n++) {
        if (!squarefree_ll(n)) continue;
        long long absD = (n % 4 == 3) ? n : 4 * n;
        if (class_number_ll(absD) == 2)
            terms[nterms++] = n;
    }
}

seq_t a(idx_t n) {
    if (nterms < 0) build_terms();
    if (n < 1 || n > (idx_t)nterms) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
