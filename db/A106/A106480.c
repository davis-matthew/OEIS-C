/*
 * A106480  Row sums of inverse of sequence array for Euler phi function.
 * https://oeis.org/A106480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1; for n≥1, a(n) = –∑_{k=1}^{n} (φ(k+1) – φ(k)) · a(n – k),
 *            where φ(m) is Euler’s totient function
 * Range:     i8 n <= 23, i16 n <= 44, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   phi_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi_of.h"

/*--------------------------------------------------------------------
  Memoised calculation of the sequence.
  The tables φ[⋅] and a[⋅] are grown as needed and can be called
  in any order.
  --------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static tables */
    static seq_t *phi   = NULL;  /* φ[i] = φ(i) */
    static idx_t phi_len = 0;
    static seq_t *a_arr = NULL;  /* a_arr[i] = a(i) */
    static idx_t a_len  = 0;

    /* initialise φ[0] and φ[1] */
    if (phi_len < 2) {
        phi_len = 2;
        phi = (seq_t *)malloc(phi_len * sizeof(seq_t));
        phi[0] = 0;          /* φ(0) – unused */
        phi[1] = 1;          /* φ(1) = 1 */
    }

    /* extend φ table up to index n+1 (needed for φ(k+1) with k=n) */
    if (phi_len <= n + 1) {
        idx_t old = phi_len;
        phi_len = n + 2;            /* indices 0 … n+1 */
        phi = (seq_t *)realloc(phi, phi_len * sizeof(seq_t));
        for (idx_t i = old; i <= n + 1; ++i) {
            phi[i] = phi_of(i);
        }
    }

    /* initialise a[0] */
    if (a_len == 0) {
        a_len = 1;
        a_arr = (seq_t *)malloc(a_len * sizeof(seq_t));
        a_arr[0] = (seq_t)1;       /* a(0) = 1 */
    }

    /* compute a[i] for all i up to n */
    if (a_len <= n) {
        idx_t old = a_len;
        a_len = n + 1;              /* indices 0 … n */
        a_arr = (seq_t *)realloc(a_arr, a_len * sizeof(seq_t));
        for (idx_t i = old; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t diff = phi[k + 1] - phi[k];
                sum += diff * a_arr[i - k];
            }
            a_arr[i] = -sum;
        }
    }

    return a_arr[n];
}
