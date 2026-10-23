/*
 * A043307  a(n) = A033001(n)/4.
 * https://oeis.org/A043307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth number (n ≥ 1) whose base‑9 representation uses
 *            only the digits 0, 1, 2 and contains no equal adjacent digits.
 *            The sequence is sorted in increasing decimal value
 * Range:     u8 n <= 14, u16 n <= 78, u32 n <= 2814, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* out of offset – safe fallback */

    /* Find the length L of the base‑9 number.
       We need the smallest L ≥ 1 such that 2^(L+1) – 2 ≥ n.
       equivalently 2^(L+1) ≥ n + 2. */

    idx_t L = 0;
    seq_t p = (seq_t)1 << 1;            /* p = 2^(L+1), start with L = 0 */

    while (p < (seq_t)n + 2) {
        ++L;
        p <<= 1;                      /* now p = 2^(L+1) again */
    }
    /* At this point 2^(L+1) = p ≥ n+2, and L is the desired length. */

    /* Index j (0‑based) of the number among all length‑L numbers. */
    seq_t pow2L = (seq_t)1 << L;        /* 2^L */
    seq_t j = (seq_t)n - pow2L + 1;     /* j = n – 2^L + 1 */

    seq_t val = 0;
    bool first = true;
    unsigned int prev = 0;              /* placeholder, overwritten on first digit */

    for (idx_t pos = 0; pos < L; ++pos) {
        unsigned int bit = (unsigned int)((j >> (L - 1 - pos)) & 1u);

        unsigned int digit;
        if (first) {                    /* first digit: 1 if bit==0, else 2 */
            digit = bit ? 2 : 1;
            first = false;
        } else {                        /* choose between the two digits
                                           allowed after the previous one. */
            unsigned int a, b;
            switch (prev) {
                case 0: a = 1; b = 2; break;          /* prev 0 → digits 1,2 */
                case 1: a = 0; b = 2; break;          /* prev 1 → digits 0,2 */
                default: a = 0; b = 1; break;        /* prev 2 → digits 0,1 */
            }
            digit = bit ? b : a;
        }

        val = val * 9 + digit;
        prev = digit;
    }

    return val;
}
