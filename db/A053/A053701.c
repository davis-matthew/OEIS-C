/*
 * A053701  Vertically symmetric numbers.
 * https://oeis.org/A053701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 84, u32 n <= 3749, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow5_ui
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): nth vertically symmetric number – numbers whose decimal
 * representation is symmetric with respect to a vertical mirror.
 * (digit mapping: 0↔0, 1↔1, 8↔8, 2↔5). */

#include <stdint.h>
#include "../utils/pow5_ui.h"

seq_t a(idx_t n)
{
    /* The first three values are handled directly. */
    if (n <= 3) {
        static const seq_t first3[3] = {0, 1, 8};
        return first3[n - 1];
    }

    /* Count of numbers for length 1: 0,1,8 */
    idx_t cum = 3;
    idx_t L = 2;

    /* Find the length L such that cumulative count >= n. */
    for (;; ++L) {
        idx_t half = L / 2;
        idx_t block = 4 * pow5_ui(half - 1);
        if (L & 1) block *= 3;            /* middle digit choices */
        if (n <= cum + block) break;       /* found correct length */
        cum += block;
    }

    /* Offset within this length block (0‑based). */
    idx_t offset = n - cum - 1;
    idx_t half = L / 2;
    idx_t middleFactor = (L & 1) ? 3 : 1;    /* 3 for odd, 1 for even */

    /* Leading digit selection (4 possibilities). */
    static const int leadingDigits[4] = {1, 2, 5, 8};
    idx_t blockPerLeading = pow5_ui(half - 1) * middleFactor;
    idx_t leadingIndex = offset / blockPerLeading;
    offset %= blockPerLeading;

    int leftHalf[16];                      /* enough for all L <= 11 */
    leftHalf[0] = leadingDigits[leadingIndex];

    /* Remaining left‑half digits (each has 5 possibilities). */
    static const int otherDigits[5] = {0, 1, 2, 5, 8};
    for (idx_t pos = 1; pos < half; ++pos) {
        idx_t remainingPow = pow5_ui(half - 1 - pos);
        idx_t subBlocks = remainingPow * middleFactor;
        idx_t digitIndex = offset / subBlocks;
        offset %= subBlocks;
        leftHalf[pos] = otherDigits[digitIndex];
    }

    /* Middle digit if length is odd. */
    int middleDigit = 0;
    if (L & 1) {
        static const int middleDigits[3] = {0, 1, 8};
        middleDigit = middleDigits[offset];   /* offset < 3 */
    }

    /* Construct the resulting number. */
    seq_t result = 0;
    for (idx_t i = 0; i < half; ++i) {
        result = result * 10 + (seq_t)leftHalf[i];
    }
    if (L & 1) {
        result = result * 10 + (seq_t)middleDigit;
    }
    for (long long i = (long long)half - 1; i >= 0; --i) {
        int d = leftHalf[i];
        int mirror = (d == 2) ? 5 : (d == 5) ? 2 : d;
        result = result * 10 + (seq_t)mirror;
    }

    return result;
}
