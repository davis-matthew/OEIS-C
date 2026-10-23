/*
 * A178787  Number of numbers <= n having distinct digits in their decimal
 *          representation, cf. A010784.
 * https://oeis.org/A178787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   Count of numbers <= n whose decimal digits are all distinct
 *            (including zero). The algorithm enumerates all numbers with
 *            fewer digits and then counts those with the same length by
 *            examining each position with a combinatorial argument
 * Range:     u8 n <= 325, u16 n <= 9886, u32 n <= 9886, u64 n <= 9886,
 *            u128 n <= 9886, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Permutation product m*(m-1)*...*(m-k+1) */
static seq_t perm(uint8_t m, uint8_t k) {
    seq_t r = 1;
    for (uint8_t i = 0; i < k; ++i) r *= (m - i);
    return r;
}

/* Return a(n): number of integers x with 0 <= x <= n having distinct
   decimal digits. */
seq_t a(idx_t n) {
    /* The largest number with all distinct decimal digits is
       9876543210.  For any larger n the count is the same. */
    const idx_t MAX_DISTINCT = 9876543210ULL;
    if (n > MAX_DISTINCT) n = MAX_DISTINCT;

    /* Convert n to digits (most significant first). */
    int digits[20];          /* 20 is enough for 64‑bit indices. */
    int len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[len++] = 0;
    } else {
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        /* Reverse to get most significant first. */
        for (int i = 0; i < len / 2; ++i) {
            int t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    seq_t count = 0;

    /* Count numbers with fewer digits. */
    for (int l = 1; l < len; ++l) {
        if (l == 1) {
            count += (seq_t)10;                 /* 0..9 */
        } else {
            count += (seq_t)9 * perm(9, l - 1); /* 9 choices for first digit,
                                                   then permutations of 9 digits. */
        }
    }

    /* Count numbers with the same length. */
    bool used[10] = {0};
    for (int i = 0; i < len; ++i) {
        int start = 0;
        if (len > 1 && i == 0)          /* leading zero not allowed for length > 1 */
            start = 1;
        for (int d = start; d < digits[i]; ++d) {
            if (!used[d]) {
                count += perm(10 - (i + 1), len - i - 1);
            }
        }
        if (used[digits[i]])            /* duplicate digit -> stop */
            return count;
        used[digits[i]] = true;
    }

    /* Include n itself (it has distinct digits). */
    return count + 1;
}
