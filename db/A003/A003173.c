/*
 * A003173  Heegner numbers: imaginary quadratic fields with unique
 *          factorization (or class number 1).
 * https://oeis.org/A003173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   search over squarefree d >= 1, keeping those where the class
 *            number of Q(sqrt(-d)) is 1, i.e. there is exactly one reduced
 *            primitive binary quadratic form of fundamental discriminant D (D
 *            = -d if d == 3 mod 4, else D = -4d). The sequence is finite (9
 *            terms); returns 0 beyond it
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-02)
 * Helpers:   is_squarefree_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_squarefree_i64.h"

static long long a3173_gcd(long long x, long long y)
{
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    while (y) { long long t = x % y; x = y; y = t; }
    return x;
}

/* number of reduced primitive forms (a,b,c), b^2 - 4ac = -N */
static long long a3173_classno(long long N)
{
    long long h = 0, a, b, c;
    for (a = 1; 3 * a * a <= N; a++) {
        for (b = -a + 1; b <= a; b++) {
            long long t = b * b + N;
            if (t % (4 * a) != 0) continue;
            c = t / (4 * a);
            if (c < a) continue;
            if (c == a && b < 0) continue;
            if (a3173_gcd(a3173_gcd(a, b), c) != 1) continue;
            h++;
        }
    }
    return h;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    long long d;
    /* Heegner-Stark: none beyond 163; search a safe bound */
    for (d = 1; d <= 2000; d++) {
        long long N;
        if (!is_squarefree_i64(d)) continue;
        N = (d % 4 == 3) ? d : 4 * d;
        if (a3173_classno(N) == 1) {
            cnt++;
            if (cnt == n) return (seq_t)d;
        }
    }
    return 0;
}
