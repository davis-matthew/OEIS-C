/*
 * A036435  Digits are nonzero squares.
 * https://oeis.org/A036435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n‑th positive integer whose decimal digits are * the
 *            non‑zero squares {1,4,9}. The sequence is obtained by listing *
 *            all strings over {1,4,9} in increasing length and lexicographic
 *            order, * then interpreting each as a decimal number
 * Range:     u8 n <= 21, u16 n <= 282, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t digit_map[3] = {1, 4, 9};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, fallback */

    /* Find the length L of the n‑th number.  There are 3^k numbers of
     * length k.  We accumulate counts until we reach or pass n. */
    idx_t sum = 0;                  /* numbers of shorter lengths */
    idx_t pow3 = 3;                 /* 3^1 */
    int L = 1;                      /* current length */

    while (sum + pow3 < n) {
        sum += pow3;
        if (pow3 > UINT64_MAX / 3) break;  /* avoid overflow */
        pow3 *= 3;
        ++L;
    }

    idx_t offset = n - sum - 1; /* 0‑based offset within length‑L block */

    /* Convert offset to base‑3 with L digits, padding with 0, and map
     * each base‑3 digit to {1,4,9}.  Store in reverse order. */
    uint8_t rev[64];              /* L will never exceed this for n up to 10^19 */
    int i;
    for (i = 0; i < L; ++i) {
        rev[i] = digit_map[offset % 3];
        offset /= 3;
    }

    /* Build the decimal value from the most significant digit to the
     * least significant digit. */
    seq_t val = 0;
    for (int j = L - 1; j >= 0; --j) {
        val = val * 10 + rev[j];
    }

    return val;
}
