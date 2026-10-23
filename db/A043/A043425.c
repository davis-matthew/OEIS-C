/*
 * A043425  Numbers having one 1 in base 8.
 * https://oeis.org/A043425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043425: Numbers having exactly one digit 1 in base 8.
   a(n) is the n‑th positive integer whose base‑8 representation
   contains exactly one occurrence of the digit 1. */

#include <stdbool.h>

static seq_t pow7[20];
static seq_t pow8[20];
static bool initialized = false;

/* Precompute powers of 7 and 8 up to the 19th exponent (enough for n≤10000). */
static void init_pow(void) {
    if (initialized) return;
    pow7[0] = 1;
    for (unsigned int i = 1; i < 20; ++i) pow7[i] = pow7[i-1] * 7;
    pow8[0] = 1;
    for (unsigned int i = 1; i < 20; ++i) pow8[i] = pow8[i-1] * 8;
    initialized = true;
}

/* Count how many (rem)-digit suffixes are possible when we already
   (or not) used the mandatory digit 1. */
static seq_t count_sequences(unsigned int rem, bool used_one) {
    if (rem == 0) return used_one ? 1 : 0;
    if (used_one) return pow7[rem];
    return (seq_t)rem * pow7[rem-1];
}

seq_t a(idx_t n) {
    init_pow();
    if (n == 0) return 0; /* sequence starts at n = 1 */

    /* Find the block (length L) that contains the n-th term. */
    seq_t remaining = (seq_t)n;
    unsigned int L = 1;
    for (;;) {
        seq_t block_size;
        if (L == 1) {
            block_size = 1;
        } else {
            /* For L ≥ 2:
               count(L) = 7^(L-2) * (6·L + 1) */
            block_size = pow7[L-2] * ((seq_t)(6 * L + 1));
        }
        if (remaining > block_size) {
            remaining -= block_size;
            ++L;
        } else {
            break;
        }
    }

    /* 0‑based offset within the block of length L */
    seq_t offset = remaining - 1;
    unsigned int rem = L - 1; /* digits after the most significant one */

    /* Determine the most significant digit (MSD). */
    seq_t count_msd = pow7[rem];          /* cases with MSD = 1 */
    int msd;
    bool used_one;
    if (offset < count_msd) {
        msd = 1;
        used_one = true;
    } else {
        offset -= count_msd;
        seq_t each = rem * pow7[rem-1];   /* each non‑1 MSD contributes this many numbers */
        msd = 2;
        used_one = false;
        while (offset >= each) {
            offset -= each;
            ++msd;
        }
    }

    /* Build the value */
    seq_t value = (seq_t)msd * pow8[rem];

    /* Fill the remaining digits from the highest (rem‑1) to 0. */
    for (int pos = (int)rem - 1; pos >= 0; --pos) {
        for (int cand = 0; cand <= 7; ++cand) {
            if (cand == 1 && used_one) continue;
            bool new_used_one = used_one || (cand == 1);
            seq_t ways = count_sequences((unsigned int)pos, new_used_one);
            if (offset < ways) {
                value += (seq_t)cand * pow8[pos];
                used_one = new_used_one;
                break;
            } else {
                offset -= ways;
            }
        }
    }
    return value;
}
