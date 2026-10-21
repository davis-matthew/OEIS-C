/*
 * A216476  Number of iterations of the "Oware" operation until the initial
 *          position n is reproduced.
 * https://oeis.org/A216476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 188, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of iterations of the Oware operation until the
   initial position (digits array and pivot) reappears. */
#include <stdbool.h>

#define MAX_DIGITS 20

seq_t a(idx_t n) {
    /* 1. Convert n into an array of its decimal digits: most
       significant digit first.  The array will never exceed
       MAX_DIGITS (20 for a 64‑bit value). */
    seq_t init[MAX_DIGITS];
    uint8_t rev[MAX_DIGITS];
    int len = 0;
    uint64_t m = (uint64_t)n;
    /* At least one digit (n ≥ 1). */
    do {
        rev[len++] = (uint8_t)(m % 10);
        m /= 10;
    } while (m > 0);
    for (int i = 0; i < len; ++i)
        init[len - 1 - i] = (seq_t)rev[i];

    /* 2. Prepare a mutable copy of the digits and the pivot index. */
    seq_t cur[MAX_DIGITS];
    for (int i = 0; i < len; ++i)
        cur[i] = init[i];
    int pivot = 0;          /* Initially the first digit.  */
    seq_t count = 0;        /* Number of iterations performed. */

    /* 3. Iterate until we return to the original configuration
       (digits equal to init and pivot == 0).  */
    while (true) {
        seq_t d = cur[pivot];      /* Value to distribute. */
        cur[pivot] = 0;            /* Pivot becomes zero. */
        uint64_t steps = (uint64_t)d;  /* d is guaranteed small (≤ sum of digits). */

        /* Distribute d increments cyclically over the other digits. */
        for (uint64_t k = 1; k <= steps; ++k) {
            int idx = (pivot + (int)k) % len;
            cur[idx] = cur[idx] + 1;
        }

        pivot = (pivot + (int)steps) % len;  /* New pivot. */
        ++count;

        /* Check for return to the starting configuration. */
        if (pivot == 0) {
            bool equal = true;
            for (int i = 0; i < len; ++i) {
                if (cur[i] != init[i]) { equal = false; break; }
            }
            if (equal) break;
        }
    }

    return count;
}
