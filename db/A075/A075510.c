/*
 * A075510  Fifth column of triangle A075497.
 * https://oeis.org/A075510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 64*4^n + 486*6^n - 1024*8^n + 625*10^n) / 24 (OEIS
 *            A075510)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pow2  = NULL;
static seq_t *pow4  = NULL;
static seq_t *pow6  = NULL;
static seq_t *pow8  = NULL;
static seq_t *pow10 = NULL;
static size_t pow_len = 0;

static void ensure_size(idx_t n)
{
    if (n < pow_len) return;
    size_t new_len = (size_t)n + 1;
    pow2  = (seq_t*)realloc(pow2,  new_len * sizeof(seq_t));
    pow4  = (seq_t*)realloc(pow4,  new_len * sizeof(seq_t));
    pow6  = (seq_t*)realloc(pow6,  new_len * sizeof(seq_t));
    pow8  = (seq_t*)realloc(pow8,  new_len * sizeof(seq_t));
    pow10 = (seq_t*)realloc(pow10, new_len * sizeof(seq_t));

    if (pow_len == 0) {
        pow2[0]  = (seq_t)1;
        pow4[0]  = (seq_t)1;
        pow6[0]  = (seq_t)1;
        pow8[0]  = (seq_t)1;
        pow10[0] = (seq_t)1;
        pow_len = 1;
    }

    for (size_t i = pow_len; i < new_len; ++i) {
        pow2[i]  = pow2[i-1]  * (seq_t)2;
        pow4[i]  = pow4[i-1]  * (seq_t)4;
        pow6[i]  = pow6[i-1]  * (seq_t)6;
        pow8[i]  = pow8[i-1]  * (seq_t)8;
        pow10[i] = pow10[i-1] * (seq_t)10;
    }
    pow_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_size(n);

    seq_t num = pow2[n];
    num -= (seq_t)64  * pow4[n];
    num += (seq_t)486 * pow6[n];
    num -= (seq_t)1024 * pow8[n];
    num += (seq_t)625  * pow10[n];

    return num / (seq_t)24;
}
