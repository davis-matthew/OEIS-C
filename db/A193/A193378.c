/*
 * A193378  Imaginary part of g.f. C(x) that satisfies: C(x) = 1 + x*C(I*x)^2.
 * https://oeis.org/A193378
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the imaginary part of the coefficient of C(x) in the
 *            power series * satisfying C(x) = 1 + x * C(i*x)^2. * Recurrence
 *            for c_n = c_n^R + i*c_n^I: * c_0 = 1 * c_n = i^{n-1} *
 *            Σ_{k=0}^{n-1} c_k * c_{n-1-k} (n ≥ 1) * The function returns the
 *            imaginary part c_n^I for n ≥ 2
 * Range:     i8 n <= 9, i16 n <= 15, i32 n <= 26, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *real_part = NULL;
static seq_t *imag_part = NULL;
static size_t  cap = 0;
static size_t  len = 0;

static void grow_seq(size_t need)
{
    if (cap < need) {
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap < need) newcap <<= 1;
        seq_t *new_real = (seq_t *)realloc(real_part, newcap * sizeof(seq_t));
        seq_t *new_imag = (seq_t *)realloc(imag_part, newcap * sizeof(seq_t));
        if (!new_real || !new_imag) return;  /* allocation failed – caller will crash */
        /* initialise new elements to zero */
        for (size_t i = cap; i < newcap; ++i) {
            new_real[i] = 0;
            new_imag[i] = 0;
        }
        real_part = new_real;
        imag_part = new_imag;
        cap = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 0;  /* values below offset are not used */

    size_t N = (size_t)n;
    if (len == 0) {
        /* initialise c_0 = 1 + 0i */
        grow_seq(1);
        real_part[0] = 1;
        imag_part[0] = 0;
        len = 1;
    }

    if (N + 1 > cap) grow_seq(N + 1);

    for (size_t k = len; k <= N; ++k) {
        size_t km1 = k - 1;
        size_t r = km1 >> 1;          /* floor(km1/2) */
        seq_t rSum = 0;
        seq_t iSum = 0;

        if ((km1 & 1) == 0) {        /* km1 even: there is a central term */
            for (size_t m = 0; m < r; ++m) {
                size_t j = km1 - m;
                seq_t a = real_part[m];
                seq_t b = imag_part[m];
                seq_t c = real_part[j];
                seq_t d = imag_part[j];
                seq_t tmpR = a * c - b * d;
                seq_t tmpI = a * d + b * c;
                rSum += tmpR + tmpR;  /* double for the symmetric pair */
                iSum += tmpI + tmpI;
            }
            /* central term (m == j) */
            seq_t a = real_part[r];
            seq_t b = imag_part[r];
            seq_t c = a;
            seq_t d = b;
            seq_t tmpR = a * c - b * d;
            seq_t tmpI = a * d + b * c;
            rSum += tmpR;
            iSum += tmpI;
        } else {                      /* km1 odd: no central term */
            for (size_t m = 0; m <= r; ++m) {
                size_t j = km1 - m;
                seq_t a = real_part[m];
                seq_t b = imag_part[m];
                seq_t c = real_part[j];
                seq_t d = imag_part[j];
                seq_t tmpR = a * c - b * d;
                seq_t tmpI = a * d + b * c;
                rSum += tmpR + tmpR;  /* double for the symmetric pair */
                iSum += tmpI + tmpI;
            }
        }

        size_t mmod4 = km1 & 3;          /* km1 mod 4 */
        switch (mmod4) {
        case 0:                         /* factor = 1   */
            real_part[k] = rSum;
            imag_part[k] = iSum;
            break;
        case 1:                         /* factor = i   */
            real_part[k] = -iSum;
            imag_part[k] = rSum;
            break;
        case 2:                         /* factor = -1  */
            real_part[k] = -rSum;
            imag_part[k] = -iSum;
            break;
        case 3:                         /* factor = -i  */
            real_part[k] = iSum;
            imag_part[k] = -rSum;
            break;
        }
    }
    len = N + 1;
    return imag_part[N];
}
