/*
 * A072203  (Number of oddly factored numbers <= n) - (number of evenly
 *          factored numbers <= n).
 * https://oeis.org/A072203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice, easy, look
 * Formula:   a(n) = 1 - Σ_{k=1..n} λ(k), λ(k) = (-1)^{Ω(k)} (Liouville)
 * Range:     i8 n <= 9836, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                 /* a(0) is irrelevant, return 0 for safety */
        return 0;

    /* Allocate smallest–prime‑factor array */
    int *spf = (int *)calloc((size_t)(n + 1), sizeof(int));
    if (spf == NULL)
        return 0;                 /* out of memory – return default */

    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {       /* i is prime */
            spf[i] = (int)i;
            if (i <= n / i) {    /* avoid overflow when computing i*i */
                idx_t ii = i * i;
                for (idx_t j = ii; j <= n; j += i)
                    if (spf[j] == 0)
                        spf[j] = (int)i;
            }
        }
    }

    /* Parity of Ω(i): 0 = even, 1 = odd */
    unsigned char *par = (unsigned char *)calloc((size_t)(n + 1), sizeof(unsigned char));
    if (par == NULL) {
        free(spf);
        return 0;
    }
    par[1] = 0;                 /* Ω(1) = 0, even */

    for (idx_t i = 2; i <= n; ++i) {
        idx_t d = i / (idx_t)spf[i];
        par[i] = par[d] ^ 1;     /* toggle parity once */
    }

    /* Compute Σ λ(k) for k = 1..n */
    seq_t sum = 1;               /* λ(1) = 1 */
    for (idx_t i = 2; i <= n; ++i)
        sum += (par[i] == 0) ? (seq_t)1 : (seq_t)(-1);

    free(spf);
    free(par);

    return (seq_t)1 - sum;      /* a(n) = 1 - Σ λ(k) */
}
