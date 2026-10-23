/*
 * A168942  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: let S(n) = a(n‑1)+a(n‑2)+…+a(n‑min(n,21)) if
 *            n ≤ 21: a(n) = 23·S(n) + 2 else if n == 22: a(22) = 23·S(22) –
 *            275 else: a(n) = 23·S(n) – 276·a(n‑22) (derived from the
 *            generating function G(t) =
 *            (t²²+2t²¹+…+2t+1)/(1–23t–…–23t²¹+276t²²)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate an array large enough for indices 0..n */
    seq_t *arr = seq_alloc_array(n + 1);
    if (!arr) return (seq_t)0;  /* unlikely, but guard against malloc failure */

    arr[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t kmax = (i < 21) ? i : 21;
        for (idx_t k = 1; k <= kmax; ++k)
            sum += arr[i - k];

        if (i <= 21) {
            arr[i] = sum * (seq_t)23 + (seq_t)2;
        } else if (i == 22) {
            arr[i] = sum * (seq_t)23 - (seq_t)275;
        } else {
            arr[i] = sum * (seq_t)23 - (seq_t)276 * arr[i - 22];
        }
    }

    seq_t result = arr[n];
    seq_free_array(arr);
    return result;
}
