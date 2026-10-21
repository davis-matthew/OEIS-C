/*
 * A063171  Dyck language interpreted as binary numbers in ascending order.
 * https://oeis.org/A063171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A071152(n)/2 (Dyck words interpreted as binary numbers)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 64, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) using only integer arithmetic.
   n,k fit in unsigned.  Return type is seq_t to hold the result. */
static seq_t binom(unsigned n, unsigned k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (unsigned i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Count the number of Dyck completions of length `rem` that start with a
   given current balance `bal` (bal = #1 - #0), never going negative
   and finishing at 0.  Assumes 0 <= bal <= rem and (rem-bal) is even. */
static seq_t countDyck(unsigned rem, int bal)
{
    if (bal < 0) return 0;
    if (rem < (unsigned)bal) return 0;
    if (((rem - bal) & 1U) != 0U) return 0;
    unsigned u = (rem - bal) >> 1;            /* number of up-steps needed */
    seq_t total = binom(rem, u);
    if (u == 0) return total;
    seq_t sub = binom(rem, u - 1);
    return total - sub;
}

/* Unrank the `idx`‑th Dyck word of length 2k (idx is 0‑based) into a
   decimal integer whose decimal digits are the binary digits of the word.
   Works for k >= 1. */
static seq_t unrankDyck(unsigned k, idx_t idx)
{
    unsigned len = 2 * k;
    unsigned pos = 0;
    int bal = 0;        /* current balance (#1 - #0) */
    seq_t value = 0;    /* resulting number built digit by digit */

    for (pos = 0; pos < len; ++pos) {
        unsigned rem = len - pos;
        seq_t cnt0 = 0;
        if (bal > 0) {
            cnt0 = countDyck(rem - 1, bal - 1);   /* paths starting with '0' */
        }
        if ((seq_t)idx < cnt0) {                 /* choose '0' */
            bal -= 1;
            value = value * 10 + 0;
        } else {                                 /* choose '1' */
            idx -= (idx_t)cnt0;
            bal += 1;
            value = value * 10 + 1;
        }
    }
    return value;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) is the empty word */

    /* Index n refers to the (n‑1)-th Dyck word counting from 1. */
    idx_t m = n - 1;

    /* Find the number of pairs k such that the m‑th word lies in that block. */
    unsigned k = 1;
    idx_t cumulative = 0;
    while (1) {
        seq_t cat = binom(2 * k, k) / (k + 1);   /* Catalan number C_k */
        idx_t cat_i = (idx_t)cat;
        if (cumulative + cat_i > m) break;
        cumulative += cat_i;
        ++k;
    }

    /* Index of the word within the block of length 2k. */
    idx_t idx_in_k = m - cumulative;
    return unrankDyck(k, idx_in_k);
}
