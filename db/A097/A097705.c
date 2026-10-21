/*
 * A097705  a(n) = 4*a(n-1) + 17*a(n-2), a(1)=1, a(2)=4.
 * https://oeis.org/A097705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 17*a(n-2) (a(1)=1, a(2)=4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses fast matrix exponentiation, works for any seq_t
   (built‑in integer types or arbitrary‑precision integer classes). */

#include <stdlib.h>

/* Helper matrix structure */
typedef struct {
    seq_t a, b, c, d;   /* represents [[a, b], [c, d]] */
} Mat;

/* Multiply two 2×2 matrices over seq_t. */
static Mat mat_mul(Mat m1, Mat m2) {
    Mat r;
    r.a = m1.a * m2.a + m1.b * m2.c;
    r.b = m1.a * m2.b + m1.b * m2.d;
    r.c = m1.c * m2.a + m1.d * m2.c;
    r.d = m1.c * m2.b + m1.d * m2.d;
    return r;
}

/* Compute M^exp where M is the recurrence matrix [[4,17],[1,0]]. */
static Mat mat_pow(idx_t exp) {
    Mat result = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };   /* identity */
    Mat base   = { (seq_t)4, (seq_t)17, (seq_t)1, (seq_t)0 };

    while (exp) {
        if (exp & 1ULL)
            result = mat_mul(result, base);
        base = mat_mul(base, base);
        exp >>= 1ULL;
    }
    return result;
}

/* Main sequence accessor */
seq_t a(idx_t n) {
    if (n == 0)   /* undefined, return 0 for safety */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)4;

    /* For n ≥ 3: a(n) = (M^(n-2)) * [a(2), a(1)]^T */
    Mat m = mat_pow(n - 2);
    seq_t a2 = (seq_t)4;   /* a(2) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    return m.a * a2 + m.b * a1;
}
