/*
 * A043457  Numbers having one 1 in base 9.
 * https://oeis.org/A043457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers having exactly one 1 in base 9
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth positive integer whose base‑9 representation
   contains precisely one digit equal to 1, enumerated in increasing order. */

#include <stdbool.h>

/* 8^k for seq_t */
static seq_t pow8(idx_t k)
{
    seq_t r = 1;
    for (idx_t i = 0; i < k; ++i)
        r = r * (seq_t)8;
    return r;
}

/* number of L‑digit base‑9 numbers with exactly one digit 1 */
static seq_t count_len(idx_t L)
{
    if (L == 1) return 1;
    seq_t p1 = pow8(L-1);           /* 8^(L-1)  */
    seq_t p2 = pow8(L-2);           /* 8^(L-2)  */
    seq_t term1 = p1;
    seq_t term2 = (seq_t)7 * (seq_t)(L-1) * p2;
    return term1 + term2;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* behaviour for n<1 does not matter */

    /* determine the digit length L of the n‑th term */
    idx_t L = 1;
    seq_t cumulative = 0;
    while (1) {
        seq_t cnt = count_len(L);
        if (cumulative + cnt >= (seq_t)n) break;
        cumulative += cnt;
        ++L;
    }

    seq_t offset = (seq_t)n - cumulative - 1;   /* 0‑based index within this L‑digit block */

    if (L == 1) return 1;                       /* only number is 1 */

    seq_t pow1 = pow8(L-1);                     /* 8^(L-1) */
    if (offset < pow1) {                        /* leading digit is 1 */
        seq_t rem = offset;
        seq_t val = 1;                          /* leading 1 */
        for (idx_t pos = 0; pos < L-1; ++pos) {
            seq_t power = pow8(L-2 - pos);      /* 8^(L-2-pos) */
            seq_t d8 = (rem / power) % (seq_t)8;
            rem = rem % power;
            seq_t d = (d8 == 0) ? 0 : d8 + 1;
            val = val * (seq_t)9 + d;
        }
        return val;
    }

    /* leading digit not 1 */
    seq_t rem = offset - pow1;
    idx_t k = L-1;                               /* positions after leading digit */

    seq_t block_per_pos = pow8(k-1);             /* 8^(k-1) */
    seq_t per_leading = (seq_t)k * block_per_pos;

    seq_t d_idx = rem / per_leading;              /* 0→2, 1→3, …, 6→8 */
    seq_t rem2 = rem % per_leading;

    seq_t leading = (seq_t)2 + d_idx;             /* leading digit */

    seq_t pos_idx = rem2 / block_per_pos;          /* position of the digit 1 */
    seq_t rem3 = rem2 % block_per_pos;

    seq_t val = leading;
    idx_t non1 = 0;
    for (idx_t i = 0; i < k; ++i) {
        if ((seq_t)i == pos_idx) {
            val = val * (seq_t)9 + 1;
        } else {
            seq_t power = pow8(k-2 - non1);
            seq_t d8 = (rem3 / power) % (seq_t)8;
            rem3 = rem3 % power;
            seq_t d = (d8 == 0) ? 0 : d8 + 1;
            val = val * (seq_t)9 + d;
            ++non1;
        }
    }
    return val;
}
