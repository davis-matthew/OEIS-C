/*
 * A032965  Numbers whose base-15 representation Sum_{i=0..m} d(i)*15^(m-i)
 *          has even d(i) for all odd i.
 * https://oeis.org/A032965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base‑15 representation has even digits
 *            at all odd positions (digits are indexed from 0)
 * Range:     u8 n <= 142, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 990 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* count_len(k): number of admissible numbers with exactly k
   base‑15 digits (k ≥ 1).  The most significant digit (index 0)
   may be 1–14; digits at odd indices must be even (0,2,…,14);
   digits at even indices may be any 0–14. */
static seq_t count_len(idx_t k)
{
    seq_t r0 = 14;         /* choices for leading digit */
    seq_t mul = 1;
    for (idx_t i = 1; i < k; ++i) {
        if (i & 1)          /* odd index */
            mul *= 8;       /* 0,2,…,14 -> 8 choices */
        else
            mul *= 15;      /* unrestricted digit */
    }
    return r0 * mul;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is 1‑based; offset is 0‑based index. */
    seq_t offset = (seq_t)n - 1;

    idx_t k = 1;
    seq_t cumulative = 0;

    /* Find the length block containing the desired index. */
    while (1) {
        seq_t cnt = count_len(k);
        if (cumulative + cnt > offset)
            break;
        cumulative += cnt;
        ++k;
    }

    /* Remaining offset within the k‑digit block */
    offset -= cumulative;

    /* Prepare radices for each position. */
    seq_t radices[64];   /* enough for all k needed for n up to 10000 */
    radices[0] = 14;
    for (idx_t i = 1; i < k; ++i)
        radices[i] = (i & 1) ? 8 : 15;

    /* Mixed‑radix conversion: offset -> digit indices. */
    seq_t digitIdx[64];
    seq_t rem = offset;
    for (idx_t i = k; i > 0; --i) {
        seq_t rad = radices[i - 1];
        digitIdx[i - 1] = rem % rad;
        rem /= rad;
    }

    /* Compose the integer value from digits. */
    seq_t value = 0;
    for (idx_t i = 0; i < k; ++i) {
        seq_t d;
        if (i == 0)
            d = 1 + digitIdx[0];                /* leading digit 1 … 14 */
        else if (i & 1)
            d = digitIdx[i] * 2;                /* even digit 0,2,…,14 */
        else
            d = digitIdx[i];                    /* unrestricted 0 … 14 */
        value = value * 15 + d;
    }

    return value;
}
