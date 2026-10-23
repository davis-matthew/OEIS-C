/*
 * A032944  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^(m-i) has
 *          d(i)=0 for all odd i.
 * https://oeis.org/A032944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 889, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑9 representation has zeros in all odd positions
   (indices 0,2,4,… counting from the most significant digit). */
#include <stdbool.h>

/* Computes a(n) as defined in OEIS A032944. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is 1‑based, n>0 for valid terms */

    /* Precompute small powers of 9 for 64‑bit arithmetic. */
    static uint64_t pow9ull[20];
    static bool pow9ull_initialized = false;
    if (!pow9ull_initialized) {
        pow9ull[0] = 1ULL;
        for (int i = 1; i < 20; ++i) pow9ull[i] = pow9ull[i-1] * 9ULL;
        pow9ull_initialized = true;
    }

    /* Precompute powers of 9 for arbitrary‑precision seq_t. */
    static seq_t pow9[80];
    static bool pow9_initialized = false;
    if (!pow9_initialized) {
        pow9[0] = (seq_t)1;
        for (int i = 1; i < 80; ++i) pow9[i] = pow9[i-1] * (seq_t)9;
        pow9_initialized = true;
    }

    /* Determine the base‑9 length L of the n‑th term. */
    uint64_t cumulative = 0;
    int L = 1;                            /* current length */
    while (1) {
        int e = (L + 1) / 2;               /* number of even indices (effective digits) */
        uint64_t block = 8ULL * pow9ull[e-1];
        if (cumulative + block >= n) break;
        cumulative += block;
        ++L;
    }

    int e = (L + 1) / 2;                 /* number of even‑position digits */
    uint64_t offset = n - cumulative - 1;/* 0‑based offset within this block */

    /* Most significant digit (must be 1 … 8). */
    uint64_t msd_index = offset / pow9ull[e-1];
    seq_t msd = (seq_t)(msd_index + 1);

    /* Remaining digits (each 0 … 8). */
    uint64_t rest = offset % pow9ull[e-1];
    seq_t digits[80];
    digits[0] = msd;
    for (int k = 1; k < e; ++k) {
        uint64_t divisor = pow9ull[e-1 - k];   /* 9^(e-1-k) */
        digits[k] = (seq_t)(rest / divisor);
        rest %= divisor;
    }

    /* Reconstruct value from collected digits. */
    seq_t value = 0;
    for (int j = 0; j < e; ++j) {
        int exp = L - 1 - 2*j;                 /* exponent for 9^(L-1-2j) */
        value += digits[j] * pow9[exp];
    }
    return value;
}
