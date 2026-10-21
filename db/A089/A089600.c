/*
 * A089600  Another lazy binary representation of n: similar to A089591 except
 *          that the single carry is performed before the increment instead of
 *          after.
 * https://oeis.org/A089600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(0)=b(0)=0; \ For n>0: \ if n odd: a(n)=10*a((n-1)/2)+1;
 *            b(n)=10*b((n-1)/2)+1; \ if n even: a(n)=10*b(n/2-1)+2;
 *            b(n)=10*a(n/2);
 * Range:     u8 n <= 11, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;
static seq_t *B = NULL;
static idx_t cap = 0;

seq_t a(idx_t n) {
    if (n >= cap) {
        idx_t new_cap = n + 1;
        if (A == NULL) {
            A = (seq_t *)malloc(new_cap * sizeof(seq_t));
            B = (seq_t *)malloc(new_cap * sizeof(seq_t));
            A[0] = (seq_t)0;
            B[0] = (seq_t)0;
            cap = 1;
        } else {
            A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            B = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
        }
        for (idx_t i = cap; i < new_cap; ++i) {
            if (i & 1) {          /* odd */
                A[i] = A[(i - 1) / 2] * (seq_t)10 + (seq_t)1;
                B[i] = B[(i - 1) / 2] * (seq_t)10 + (seq_t)1;
            } else {              /* even */
                A[i] = B[i / 2 - 1] * (seq_t)10 + (seq_t)2;
                B[i] = A[i / 2] * (seq_t)10;   /* +0 */
            }
        }
        cap = new_cap;
    }
    return A[n];
}
