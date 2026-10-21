/*
 * A081610  Number of numbers <= n having at least one 2 in their ternary
 *          representation.
 * https://oeis.org/A081610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 - (# of integers ≤ n having only the digits 0 or 1
 *            in base‑3)
 * Range:     u8 n <= 302, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Count numbers <= n using only digits 0 and 1 in base 3 (denote this count as f). */
    seq_t f = 0;                 /* f will hold the count of “no‑2” numbers. */
    bool broken = false;         /* true if we encounter a digit '2' and stop the scan. */

    /* Convert n to base-3 digits (least significant first). */
    idx_t temp = n;
    int digits[64];              /* maximum number of base‑3 digits for a 64‑bit integer is < 40. */
    int len = 0;
    do {
        digits[len++] = temp % 3;
        temp /= 3;
    } while (temp);

    /* Scan digits from most significant to least. */
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        int rem = i;             /* number of less significant positions after this one. */

        /* Compute 2^rem safely for any size of seq_t. */
        seq_t pow2 = 1;
        for (int k = 0; k < rem; ++k) {
            pow2 <<= 1;
        }

        if (d == 0) {
            /* Only digit 0 can be placed here to stay ≤ n. */
            continue;
        } else if (d == 1) {
            /* Case 1: place 0 here, choose any digits for the rest. */
            f += pow2;
            /* Case 2: place 1 here and continue scanning. */
            continue;
        } else if (d == 2) {
            /* Case 1: place 0 here => 2^rem numbers. */
            f += pow2;
            /* Case 2: place 1 here => 2^rem numbers. */
            f += pow2;
            broken = true;
            break;
        } else {
            /* In base 3, digits > 2 do not occur. */
            broken = true;
            break;
        }
    }

    /* If we scanned all digits without encountering a '2', then n itself is a “no‑2” number. */
    if (!broken) {
        f += 1;
    }

    /* The desired count is (n + 1) – f. */
    seq_t total = (seq_t)n + 1;
    return total - f;
}
