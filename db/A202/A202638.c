/*
 * A202638  y-values in the solution to x^2 - 7*y^2 = -3.
 * https://oeis.org/A202638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   mul16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A202638: y-values in solutions to x^2 - 7*y^2 = -3.
 * Recurrence:
 *   A(0)=1, A(1)=14, A(k)=16*A(k-1)-A(k-2)   for k>=2
 *   B(0)=2, B(1)=31, B(k)=16*B(k-1)-B(k-2)   for k>=2
 * Then a(2k+1)=A(k), a(2k+2)=B(k). */

#include <stdlib.h>
#include "../utils/mul16.h"

static seq_t *A = NULL;          /* sequence A (odd indices)          */
static seq_t *B = NULL;          /* sequence B (even indices)        */
static size_t sizeA = 0, capA = 0;
static size_t sizeB = 0, capB = 0;

/* Ensure array A has at least (need+1) elements. */
static void ensure_A(size_t need)
{
    if (sizeA > need) return;
    if (capA <= need) {
        size_t newcap = capA ? capA * 2 : 8;
        while (newcap <= need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        A = tmp;
        capA = newcap;
    }
    while (sizeA <= need) {
        if (sizeA == 0)          A[0] = 1;
        else if (sizeA == 1)     A[1] = 14;
        else                     A[sizeA] = mul16(A[sizeA-1]) - A[sizeA-2];
        ++sizeA;
    }
}

/* Ensure array B has at least (need+1) elements. */
static void ensure_B(size_t need)
{
    if (sizeB > need) return;
    if (capB <= need) {
        size_t newcap = capB ? capB * 2 : 8;
        while (newcap <= need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(B, newcap * sizeof(seq_t));
        B = tmp;
        capB = newcap;
    }
    while (sizeB <= need) {
        if (sizeB == 0)          B[0] = 2;
        else if (sizeB == 1)     B[1] = 31;
        else                     B[sizeB] = mul16(B[sizeB-1]) - B[sizeB-2];
        ++sizeB;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below offset, safe return */
    if (n & 1) {          /* odd index: use sequence A */
        size_t k = (n - 1) >> 1;
        ensure_A(k);
        return A[k];
    } else {              /* even index: use sequence B */
        size_t k = (n >> 1) - 1;
        ensure_B(k);
        return B[k];
    }
}
