/*
 * A063004  Difference between 2^n and the next larger or equal power of 3.
 * https://oeis.org/A063004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^ceil(log_3(2^n)) - 2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static tables for powers of 2 and 3 */
static seq_t *pow2 = NULL;
static idx_t pow2len = 0;
static seq_t *pow3 = NULL;
static idx_t pow3len = 0;

seq_t a(idx_t n) {
    /* Build powers of 2 up to n */
    if (pow2len == 0) {
        pow2len = n + 1;
        pow2 = (seq_t*)malloc(pow2len * sizeof(seq_t));
        pow2[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            pow2[i] = pow2[i - 1] * (seq_t)2;
    } else if (pow2len <= n) {
        idx_t newlen = n + 1;
        seq_t *tmp = (seq_t*)realloc(pow2, newlen * sizeof(seq_t));
        pow2 = tmp;
        for (idx_t i = pow2len; i < newlen; ++i)
            pow2[i] = pow2[i - 1] * (seq_t)2;
        pow2len = newlen;
    }

    /* Ensure the 3‑power table contains a value >= pow2[n] */
    if (pow3len == 0) {
        pow3len = 1;
        pow3 = (seq_t*)malloc(pow3len * sizeof(seq_t));
        pow3[0] = (seq_t)1;
    }
    while (pow3[pow3len - 1] < pow2[n]) {
        seq_t next = pow3[pow3len - 1] * (seq_t)3;
        seq_t *tmp = (seq_t*)realloc(pow3, (pow3len + 1) * sizeof(seq_t));
        pow3 = tmp;
        pow3[pow3len] = next;
        ++pow3len;
    }

    /* Binary search for the smallest k with 3^k >= 2^n */
    idx_t low = 0, high = pow3len - 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (pow3[mid] < pow2[n])
            low = mid + 1;
        else
            high = mid;
    }
    idx_t k = low;

    return pow3[k] - pow2[n];
}
