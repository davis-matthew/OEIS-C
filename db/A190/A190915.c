/*
 * A190915  Number of association types of a binary-ternary algebra with a
 *          commutative binary operation.
 * https://oeis.org/A190915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A190915
 *
 * Let a(1)=1.
 * For n>1 define:
 *   A[n] = BinaryPart(n) + TernaryPart(n)
 *
 *   BinaryPart(n) = (Conv[n] + (n even ? a[n/2] : 0)) / 2
 *   where Conv[n] = Σ_{i=1}^{n-1} a[i] * a[n-i]
 *
 *   TernaryPart(n) = Σ_{i=1}^{n-2} a[i] * Conv[n-i]
 *
 * The recurrence uses only integer arithmetic; it gives
 * the exact number of association types for a binary‑ternary
 * algebra with a commutative binary operation.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static tables: up to 10000 pre‑allocated */
    static seq_t  A[10001];
    static seq_t  C[10001];
    static idx_t  computed = 0;
    static int    init = 0;

    if (!init) {
        A[0] = 0;
        C[0] = 0;
        A[1] = 1;   /* a(1) = 1 */
        C[1] = 0;   /* convolution for n=1 is 0 */
        computed = 1;
        init = 1;
    }

    if (n <= computed) return A[n];

    for (idx_t i = computed + 1; i <= n; ++i)
    {
        /* compute convolution C[i] = Σ_{k=1}^{i-1} A[k]*A[i-k] */
        seq_t conv = 0;
        for (idx_t k = 1; k < i; ++k) {
            conv += A[k] * A[i - k];
        }
        C[i] = conv;

        /* binary part: (conv + (i even? A[i/2]:0)) / 2 */
        seq_t bin = (conv + ( (i & 1) == 0 ? A[i >> 1] : 0 )) / 2;

        /* ternary part: Σ_{k=1}^{i-2} A[k] * C[i-k] */
        seq_t ter = 0;
        for (idx_t k = 1; k + 1 < i; ++k) {
            ter += A[k] * C[i - k];
        }

        A[i] = bin + ter;
    }

    computed = n;
    return A[n];
}
