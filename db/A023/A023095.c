/*
 * A023095  a(n) is the least k > 0 such that k and 3k are anagrams in base n
 *          (written in base 10).
 * https://oeis.org/A023095
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k>0 such that k and 3k are anagrams in base n
 *            (written in base 10)
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Return the value a(n) for n >= 4.  For n < 4 the result is undefined
   and this function simply returns 0. */
seq_t a(idx_t n) {
    if (n < 4) return 0;

    /* 2*a(n) is divisible by n-1, so every solution k must satisfy
       2*k ≡ 0 (mod n-1).  This reduces the search step size. */
    seq_t step = (seq_t)((n - 1) / ((((n - 1) & 1) ? 1 : 2)));

    /* Arrays large enough to hold the base‑n digits of k and 3k
       (the maximum number of digits for any n <= 10000 is well below 64). */
    #define MAX_DIG 64
    uint16_t dk[MAX_DIG], dk3[MAX_DIG];

    for (seq_t k = step; ; k += step) {
        seq_t k3 = k * 3;

        /* Gather digits of k in base n. */
        seq_t tmp = k;
        size_t dklen = 0;
        while (tmp > 0) {
            dk[dklen++] = (uint16_t)(tmp % n);
            tmp /= n;
        }
        if (dklen == 0) dk[dklen++] = 0;   /* k == 0 cannot happen, but keep safety */

        /* Gather digits of 3k in base n. */
        tmp = k3;
        size_t dk3len = 0;
        while (tmp > 0) {
            dk3[dk3len++] = (uint16_t)(tmp % n);
            tmp /= n;
        }
        if (dk3len == 0) dk3[dk3len++] = 0;

        /* Different lengths → not anagrams. */
        if (dklen != dk3len) continue;

        /* Sort the digit arrays (insertion sort is fine for small sizes). */
        for (size_t i = 1; i < dklen; ++i) {
            uint16_t v = dk[i];
            size_t j = i;
            while (j > 0 && dk[j - 1] > v) {
                dk[j] = dk[j - 1];
                --j;
            }
            dk[j] = v;
        }
        for (size_t i = 1; i < dk3len; ++i) {
            uint16_t v = dk3[i];
            size_t j = i;
            while (j > 0 && dk3[j - 1] > v) {
                dk3[j] = dk3[j - 1];
                --j;
            }
            dk3[j] = v;
        }

        /* Compare the sorted digit sequences. */
        bool equal = true;
        for (size_t i = 0; i < dklen; ++i) {
            if (dk[i] != dk3[i]) {
                equal = false;
                break;
            }
        }
        if (equal) return k;
    }
}
