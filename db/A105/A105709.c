/*
 * A105709  Smallest m such that 9 is at the n-th position of the decimal
 *          representation of the m-th Fibonacci number.
 * https://oeis.org/A105709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest m such that the n‑th digit from the right of the
 *            * m‑th Fibonacci number is 9. (search by iterating Fibonacci *
 *            numbers and keeping only the lowest n+1 decimal digits)
 * Range:     u8 n <= 49, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* number of decimal places to keep (lowest n+1 digits) */
    size_t len = (size_t)n + 1;

    /* three buffers for digit vectors, least–significant digit first */
    uint8_t *A = (uint8_t *)malloc(len);
    uint8_t *B = (uint8_t *)malloc(len);
    uint8_t *C = (uint8_t *)malloc(len);
    if (!A || !B || !C) {
        free(A); free(B); free(C);
        return (seq_t)0;          /* allocation failure: unlikely in tests   */
    }

    size_t lA = 1, lB = 1;      /* current lengths of A and B          */
    A[0] = 0;                   /* F_0  */
    B[0] = 1;                   /* F_1  */

    idx_t m = 0;                /* current Fibonacci index ( F_m in A ) */

    while (1) {
        /* check if F_m (stored in A) has 9 at position n */
        if (lA > n && A[n] == 9) {
            free(A); free(B); free(C);
            return (seq_t)m;
        }

        /* compute C = (A + B) mod 10^(n+1) (kept in buffers of length len) */
        size_t maxlen = (lA > lB) ? lA : lB;
        if (maxlen < len) maxlen = len;

        uint8_t carry = 0;
        for (size_t i = 0; i < maxlen; ++i) {
            uint8_t da = (i < lA) ? A[i] : 0;
            uint8_t db = (i < lB) ? B[i] : 0;
            uint16_t sum = (uint16_t)da + (uint16_t)db + (uint16_t)carry;
            C[i] = (uint8_t)(sum % 10);
            carry = (uint8_t)(sum / 10);
        }
        size_t lC = maxlen;
        while (lC > 1 && C[lC - 1] == 0)
            --lC;

        /* rotate buffers to advance one Fibonacci step */
        uint8_t *tmp = A;  A = B;  B = C;  C = tmp;
        lA = lB;  lB = lC;

        ++m;            /* next index (A now holds F_{m}) */
    }

    /* unreachable but satisfies the compiler */
    free(A); free(B); free(C);
    return (seq_t)0;
}
