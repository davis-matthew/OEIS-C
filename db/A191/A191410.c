/*
 * A191410  Class number, k, of n, i.e.; imaginary quadratic fields negated
 *          Q(sqrt(-n))=k, or 0 if n is not a fundamental discriminant
 *          (A003657).
 * https://oeis.org/A191410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = class number of the imaginary quadratic field Q(sqrt(-n))
 *            (0 if n is not a fundamental discriminant)
 * Range:     u8 n <= 790, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* gcd for unsigned integer type idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* test whether n is squarefree (n>0) */
static bool is_squarefree(idx_t n)
{
    idx_t x = n;
    for (idx_t i = 2; i * i <= x; ++i) {
        if (x % i == 0) {
            if ((x / i) % i == 0)  /* repeated factor */
                return false;
            while (x % i == 0)
                x /= i;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* check if n is a fundamental discriminant of the form -n */
    if (n % 4 == 3) {                /* n ≡ 3 (mod 4) must be squarefree */
        if (!is_squarefree(n))
            return 0;
    } else if (n % 4 == 0) {         /* n ≡ 0 (mod 4) */
        idx_t m = n / 4;
        /* m must be squarefree and m ≡ 1 or 2 (mod 4) */
        if (!is_squarefree(m))
            return 0;
        if ((m % 4) != 1 && (m % 4) != 2)
            return 0;
    } else {                          /* n ≡ 1 or 2 (mod 4) are not fundamental */
        return 0;
    }

    /* compute class number of the imaginary quadratic field with discriminant -n */
    idx_t D = n;                /* |d| = n */
    seq_t h = 0;

    /* loop over reduced forms: |b| ≤ a ≤ c,
       a > 0, d = b^2 – 4ac = –n */
    for (idx_t a = 1; a * a <= D / 3; ++a) {
        int parity = (int)(D & 1);          /* 0 if d even, 1 if d odd */
        int b_start = -(int)a;
        if ((b_start & 1) != parity)
            b_start += 1;                  /* adjust to correct parity */

        for (int b = b_start; b <= (int)a; b += 2) {
            seq_t num = (seq_t)b * b + (seq_t)D;   /* b^2 + n */
            seq_t den = (seq_t)4 * a;
            if (num % den != 0)
                continue;                  /* not integral c */

            seq_t c = num / den;
            if ((idx_t)c < a)
                continue;                  /* must have a ≤ c */

            /* ensure reducedness */
            idx_t ab_abs = (b < 0) ? (idx_t)(-b) : (idx_t)b;
            if (((a == (idx_t)c) || (a == ab_abs)) && b < 0)
                continue;                  /* not reduced */

            /* primitive form? */
            idx_t g = gcd_idx(a, ab_abs);
            g = gcd_idx(g, (idx_t)c);
            if (g == 1)
                h += (seq_t)1;
        }
    }

    return h;
}
