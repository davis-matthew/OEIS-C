/*
 * A003644  Discriminants of the known imaginary quadratic fields with 1 class
 *          per genus (a finite sequence).
 * https://oeis.org/A003644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   search over discriminants -D (D <= 10000): a(n) = n-th D such
 *            that -D is a fundamental discriminant and every reduced
 *            primitive binary quadratic form of discriminant -D is ambiguous
 *            (b=0, b=a or a=c), i.e. the class group is an elementary 2-group
 *            (one class per genus). The sequence is finite with...
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: full, pass, 65 terms through n = 65 (2026-11-02)
 * Helpers:   gcd_i64, is_squarefree_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_i64.h"
#include "../utils/is_squarefree_i64.h"

static bool a003644_fundamental(long D)
{
    if (D % 4 == 3) return is_squarefree_i64(D);
    if (D % 4 == 0) {
        long m = D / 4;
        if (m % 4 != 1 && m % 4 != 2) return false;
        return is_squarefree_i64(m);
    }
    return false;
}

static bool a003644_one_class_per_genus(long D)
{
    long b;
    /* look for a non-ambiguous reduced primitive form: 0 < b < a < c */
    for (b = (D & 1); 3 * b * b <= D; b += 2) {
        long a, N = b * b + D;
        if (b == 0) continue;
        for (a = b + 1; 4 * a * a < N; a++) {
            long c;
            if (N % (4 * a) != 0) continue;
            c = N / (4 * a);
            if (c <= a) continue;
            if (gcd_i64(gcd_i64(a, b), c) == 1) return false;
        }
    }
    return true;
}

static long a003644_terms[128];
static int a003644_count = -1;

seq_t a(idx_t n)
{
    if (a003644_count < 0) {
        long D;
        int cnt = 0;
        for (D = 3; D <= 10000 && cnt < 128; D++)
            if (a003644_fundamental(D) && a003644_one_class_per_genus(D))
                a003644_terms[cnt++] = D;
        a003644_count = cnt;
    }
    if (n < 1 || n > (idx_t)a003644_count) return (seq_t)0;
    return (seq_t)a003644_terms[n - 1];
}
