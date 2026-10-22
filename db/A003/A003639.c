/*
 * A003639  Number of classes per genus in Q(sqrt -n), n squarefree.
 * https://oeis.org/A003639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(D) / 2^(omega(|D|)-1), where m = A005117(n) (the n-th
 *            squarefree number), D = -m if m == 3 (mod 4) else D = -4m, h(D)
 *            = class number (counted as reduced binary quadratic forms),
 *            omega = number of distinct prime factors
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   is_squarefree_ll, nth_squarefree, omega_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_squarefree_ll.h"
#include "../utils/nth_squarefree.h"
#include "../utils/omega_ll.h"

seq_t a(idx_t n) {
    long long m = nth_squarefree(n);
    long long absD = (m % 4 == 3) ? m : 4 * m;
    long long h = 0, A, b;
    int t;
    for (A = 1; 3 * A * A <= absD; A++) {
        for (b = -A + 1; b <= A; b++) {
            long long num = b * b + absD;
            long long c;
            if (num % (4 * A) != 0) continue;
            c = num / (4 * A);
            if (c < A) continue;
            if (c == A && b < 0) continue;
            h++;
        }
    }
    t = omega_ll(absD);
    return (seq_t)(h >> (t - 1));
}
