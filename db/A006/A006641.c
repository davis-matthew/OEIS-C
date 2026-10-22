/*
 * A006641  Class number of forms with discriminant -A003657(n), or
 *          equivalently class number of imaginary quadratic field with
 *          discriminant -A003657(n).
 * https://oeis.org/A006641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = h(-A003657(n)), the number of reduced primitive binary
 *            quadratic forms (a,b,c) with b^2-4ac = -A003657(n), |b| <= a <=
 *            c, b >= 0 if |b| = a or a = c. Computed by search: enumerate
 *            fundamental discriminants, then count reduced forms
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool is_squarefree(uint64_t m) {
    if (m < 1) return false;
    for (uint64_t p = 2; p * p <= m; p++)
        if (m % (p * p) == 0) return false;
    return true;
}

/* Absolute value of the n-th fundamental discriminant < 0, i.e. A003657(n). */
static int64_t get_discriminant(idx_t n) {
    idx_t count = 0;
    int64_t D = 3;
    for (;;) {
        bool ok = false;
        if (D % 4 == 3) {
            ok = is_squarefree((uint64_t)D);
        } else if (D % 4 == 0) {
            int64_t m = D / 4;
            if ((m % 4 == 1 || m % 4 == 2) && is_squarefree((uint64_t)m)) ok = true;
        }
        if (ok) {
            count++;
            if (count == n) return D;
        }
        D++;
    }
}

static seq_t class_number(int64_t N) {
    seq_t count = 0;
    for (int64_t a = 1; 3 * a * a <= N; a++) {
        for (int64_t b = -a + 1; b <= a; b++) {
            int64_t num = b * b + N;
            if (num % (4 * a) != 0) continue;
            int64_t c = num / (4 * a);
            if (c < a) continue;
            if (a == c && b < 0) continue;
            count++;
        }
    }
    return count;
}

seq_t a(idx_t n) {
    return class_number(get_discriminant(n));
}
