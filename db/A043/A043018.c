/*
 * A043018  Base-7 palindromes that start with 4.
 * https://oeis.org/A043018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Base‑7 palindromes that start with digit 4 (first digit 4, palindrome) */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)4;                /* L = 1 */

    /* Precompute powers of 7 up to exponent 10 (enough for n up to >10⁴) */
    static seq_t pow7[20];
    static int pow7_done = 0;
    if (!pow7_done) {
        pow7[0] = (seq_t)1;
        for (int i = 1; i < 20; ++i) pow7[i] = pow7[i - 1] * (seq_t)7;
        pow7_done = 1;
    }

    /* Find the length L of the desired palindrome */
    seq_t cum = 1;            /* count for L = 1 */
    int L = 1;
    seq_t cnt = 1;
    seq_t target = (seq_t)n;
    while (cum < target) {
        ++L;
        if (L == 1) {
            cnt = (seq_t)1;
        } else if (L % 2 == 0) {                     /* even L = 2*m */
            int m = L / 2;
            int exp = m - 1;
            cnt = pow7[exp];
        } else {                                    /* odd L = 2*m+1 */
            int exp = L / 2;                         /* floor(L/2) */
            cnt = pow7[exp];
        }
        cum += cnt;
    }
    seq_t cum_prev = cum - cnt;                     /* cumulative before L */

    /* Index within palindromes of length L (0‑based) */
    seq_t idx_within_seq = target - cum_prev - 1;
    /* idx_within fits in a 64‑bit integer because n ≤ 10⁴ */
    idx_t  idx_within = (idx_t)idx_within_seq;

    /* Build base‑7 digits of the palindrome */
    int half = L / 2;                                /* floor(L/2) */
    int free_count = 0;
    if (L == 1) free_count = 0;
    else if (L % 2 == 0) free_count = half - 1;      /* even: free digits after first */
    else              free_count = half;            /* odd : free digits after first plus middle */

    int digits[32] = {0};
    digits[0] = 4;                                   /* first digit fixed */

    /* Extract free digits from index (least significant digit to most) */
    int free_digs[32] = {0};
    for (int i = free_count - 1; i >= 0; --i) {
        free_digs[i] = idx_within % 7;
        idx_within /= 7;
    }

    if (L == 1) {
        return (seq_t)4;
    }

    if (L % 2 == 0) {                                 /* even length */
        int first_half_len = half;                    /* positions 0 .. half-1 */
        for (int i = 1; i < first_half_len; ++i) {
            digits[i] = free_digs[i - 1];
        }
        /* Mirror to the second half */
        for (int i = 0; i < first_half_len; ++i) {
            digits[L - 1 - i] = digits[i];
        }
    } else {                                          /* odd length */
        int first_half_len = half + 1;                /* positions 0 .. half */
        for (int i = 0; i < free_count; ++i) {
            digits[1 + i] = free_digs[i];
        }
        /* Mirror (exclude the middle digit at position 'half') */
        for (int i = 0; i < half; ++i) {
            digits[L - 1 - i] = digits[i];
        }
    }

    /* Convert base‑7 digits to decimal (seq_t) */
    seq_t result = 0;
    for (int i = 0; i < L; ++i) {
        result = result * (seq_t)7 + (seq_t)digits[i];
    }
    return result;
}
