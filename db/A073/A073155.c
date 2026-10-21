/*
 * A073155  Leftmost column sequence of triangle A073153.
 * https://oeis.org/A073155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(n) = ∑_{i=0}^{n-1} (a(i)+a(i-1)) · (a(n-1-i)+a(n-1-i-1)),
   with a(-1) = 0
   (equivalent to a(n) = ∑_{i=0}^{n-1} b(i) · b(n-1-i),
   where b(k) = a(k) + a(k-1), b(0)=1.) */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> A, B;   /* C++ mode: use vectors, automatically constructed */
    size_t old_len = A.size();
    size_t new_len = (size_t)n + 1;
    if (old_len < new_len) {
        A.resize(new_len);
        B.resize(new_len);
        for (idx_t i = (idx_t)old_len; i <= n; ++i) {
            if (i == 0) {
                A[0] = 1;
                B[0] = 1;
            } else {
                seq_t sum = 0;
                for (idx_t j = 0; j < i; ++j) {
                    sum += B[(size_t)j] * B[(size_t)(i - 1 - j)];
                }
                A[(size_t)i] = sum;
                B[(size_t)i] = A[(size_t)i] + A[(size_t)(i - 1)];
            }
        }
    }
    return A[(size_t)n];
#else
    static seq_t *A = NULL;    /* C mode: plain malloc/realloc */
    static seq_t *B = NULL;
    static idx_t len = 0;
    size_t new_len = (size_t)n + 1;
    if (len < new_len) {
        A = (seq_t *)realloc(A, new_len * sizeof(seq_t));
        B = (seq_t *)realloc(B, new_len * sizeof(seq_t));
        for (idx_t i = len; i <= n; ++i) {
            if (i == 0) {
                A[0] = 1;
                B[0] = 1;
            } else {
                seq_t sum = 0;
                for (idx_t j = 0; j < i; ++j) {
                    sum += B[j] * B[i - 1 - j];
                }
                A[i] = sum;
                B[i] = A[i] + A[i - 1];
            }
        }
        len = new_len;
    }
    return A[n];
#endif
}
