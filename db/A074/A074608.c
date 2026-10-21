/*
 * A074608  a(n) = 3^n + 7^n.
 * https://oeis.org/A074608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *pow3 = NULL;
    static seq_t *pow7 = NULL;
    static idx_t sz3 = 0;
    static idx_t sz7 = 0;

    /* Initialise the base cases for n = 0 */
    if (sz3 == 0) {
        pow3 = (seq_t *)malloc(sizeof(seq_t));
        pow7 = (seq_t *)malloc(sizeof(seq_t));
        if (!pow3 || !pow7) return (seq_t)0;  /* safe fallback */
        pow3[0] = (seq_t)1;
        pow7[0] = (seq_t)1;
        sz3 = sz7 = 1;
    }

    /* Extend the pre‑computed tables if necessary */
    if (n >= sz3) {
        idx_t new_sz = sz3;
        while (n >= new_sz) new_sz *= 2;          /* double until enough space */

        pow3 = (seq_t *)realloc(pow3, sizeof(seq_t) * new_sz);
        pow7 = (seq_t *)realloc(pow7, sizeof(seq_t) * new_sz);
        if (!pow3 || !pow7) return (seq_t)0;      /* safe fallback */

        for (idx_t i = sz3; i < new_sz; ++i) {
            pow3[i] = pow3[i - 1] * (seq_t)3;
            pow7[i] = pow7[i - 1] * (seq_t)7;
        }
        sz3 = sz7 = new_sz;
    }

    /* Result may overflow seq_t – the caller will see the wrapped value,
       which is acceptable if the value is beyond seq_t’s range. */
    return pow3[n] + pow7[n];
}
