/*
 * A108168  Padovan sequence for indices of the Beatty sequence of the
 *          tribonacci constant.
 * https://oeis.org/A108168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) = Padovan(floor(n * τ) + 2) where τ ≈
 *            1.83928675521416113267689163751126 (tribonacci constant),
 *            Padovan is defined by P(0)=0, P(1)=0, P(2)=1 and
 *            P(k)=P(k-2)+P(k-3) for k≥3. The sequence starts a(1)=0, a(2)=1,
 *            a(3)=2, a(4)=3, a(5)=5, a(6)=9, …
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n<1, but safe */

    /* Tribonacci constant (high precision) */
    const double trib_const = 1.83928675521416113267689163751126;

    /* Compute index k = floor(n*τ)+2 */
    idx_t idx = (idx_t)((double)n * trib_const + 1e-12); /* small epsilon */
    idx_t k   = idx + 2;

    /* Static Padovan array: Padovan[0]=0, Padovan[1]=0, Padovan[2]=1 */
    static seq_t *pad = NULL;
    static idx_t   pad_last = 0;   /* largest index computed */
    static size_t  pad_cap  = 0;   /* allocated capacity */

    if (pad == NULL) {
        pad_cap = 32;
        pad = (seq_t*)malloc(pad_cap * sizeof(seq_t));
        pad[0] = (seq_t)0;
        pad[1] = (seq_t)0;
        pad[2] = (seq_t)1;
        pad_last = 2;
    }

    if (k > pad_last) {
        /* Ensure capacity */
        if ((size_t)k >= pad_cap) {
            size_t new_cap = pad_cap;
            while (new_cap <= (size_t)k) new_cap <<= 1;
            pad = (seq_t*)realloc(pad, new_cap * sizeof(seq_t));
            pad_cap = new_cap;
        }
        /* Compute missing Padovan values */
        for (idx_t i = pad_last + 1; i <= k; ++i) {
            pad[i] = pad[i - 2] + pad[i - 3];
        }
        pad_last = k;
    }

    return pad[k];
}
