/*
 * A210876  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210877; see the Formula section.
 * https://oeis.org/A210876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   U(1,x)=1, V(1,x)=1; U(n,x)=x*U(n-1,x)+V(n-1,x)+1,
 *            V(n,x)=x*U(n-1,x)+x*V(n-1,x)+x. a(n) is the flattened row‑major
 *            list of coefficients of U(n,x)
 * Range:     u8 n <= 51, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* out of range indices are undefined */

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* 0‑based column within the row */
    idx_t k = n - (r - 1) * r / 2 - 1;

    /* Allocate and compute rows up to r */
#ifdef __cplusplus
    std::vector<seq_t> Uprev(1), Vprev(1);
    Uprev[0] = (seq_t)1;
    Vprev[0] = (seq_t)1;

    for (idx_t row = 2; row <= r; ++row) {
        std::vector<seq_t> Ucurr(row), Vcurr(row);
        for (idx_t j = 0; j < row; ++j) {
            seq_t u;
            if (j == 0)
                u = Vprev[0] + (seq_t)1;
            else
                u = Uprev[j - 1] + ((j < row - 1) ? Vprev[j] : (seq_t)0);
            Ucurr[j] = u;

            seq_t v;
            if (j == 0)
                v = 0;
            else
                v = Uprev[j - 1] + Vprev[j - 1] + ((j == 1) ? (seq_t)1 : (seq_t)0);
            Vcurr[j] = v;
        }
        Uprev.swap(Ucurr);
        Vprev.swap(Vcurr);
    }
    return Uprev[(size_t)k];
#else
    seq_t *Uprev = (seq_t *)malloc(sizeof(seq_t) * 1);
    seq_t *Vprev = (seq_t *)malloc(sizeof(seq_t) * 1);
    Uprev[0] = (seq_t)1;
    Vprev[0] = (seq_t)1;

    for (idx_t row = 2; row <= r; ++row) {
        idx_t sz = row;
        seq_t *Ucurr = (seq_t *)malloc(sizeof(seq_t) * sz);
        seq_t *Vcurr = (seq_t *)malloc(sizeof(seq_t) * sz);
        for (idx_t j = 0; j < sz; ++j) {
            seq_t u;
            if (j == 0)
                u = Vprev[0] + (seq_t)1;
            else
                u = Uprev[j - 1] + ((j < sz - 1) ? Vprev[j] : (seq_t)0);
            Ucurr[j] = u;

            seq_t v;
            if (j == 0)
                v = 0;
            else
                v = Uprev[j - 1] + Vprev[j - 1] + ((j == 1) ? (seq_t)1 : (seq_t)0);
            Vcurr[j] = v;
        }
        free(Uprev);
        free(Vprev);
        Uprev = Ucurr;
        Vprev = Vcurr;
    }
    seq_t result = Uprev[(size_t)k];
    free(Uprev);
    free(Vprev);
    return result;
#endif
}
