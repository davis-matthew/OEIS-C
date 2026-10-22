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
 * Helpers:   a3173_classno, a3173_gcd, a3173_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a3173_classno.h"
#include "../utils/a3173_gcd.h"
#include "../utils/a3173_squarefree.h"

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    long long d;
    /* Heegner-Stark: none beyond 163; search a safe bound */
    for (d = 1; d <= 2000; d++) {
        long long N;
        if (!a3173_squarefree(d)) continue;
        N = (d % 4 == 3) ? d : 4 * d;
        if (a3173_classno(N) == 1) {
            cnt++;
            if (cnt == n) return (seq_t)d;
        }
    }
    return 0;
}
