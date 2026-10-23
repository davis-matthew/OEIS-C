/*
 * A032930  Numbers whose set of base-9 digits is {1,2}.
 * https://oeis.org/A032930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑9 digits consist only of 1 and 2. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the length L such that the cumulative count 2^(L+1)-2 >= n. */
    idx_t cumulative = 0;    /* sum of 2^1, 2^2, …, 2^L */
    idx_t power_of_two = 2;  /* 2^1 */
    idx_t len = 1;

    while (1) {
        cumulative += power_of_two;      /* add 2^len */
        if (cumulative >= n)            /* reached enough terms */
            break;
        ++len;
        power_of_two <<= 1;              /* next 2^len */
    }

    /* offset within length‑len block (1 … 2^len) */
    idx_t previous_cumulative = cumulative - power_of_two; /* 2^len - 2 */
    idx_t offset = n - previous_cumulative;
    idx_t m = offset - 1;                 /* 0 … 2^len-1 */

    /* Precompute powers of 9 up to len-1 and their sum (all‑1’s number). */
    seq_t pow9[64] = {0};                 /* len will never exceed 63 */
    seq_t base = 1;
    seq_t sum = 0;
    unsigned int i;
    for (i = 0; i < (unsigned int)len; ++i) {
        pow9[i] = base;
        sum += base;
        base *= (seq_t)9;
    }

    /* Add 9^i for each bit that is set in m (digit 2). */
    seq_t result = sum;                  /* start with all digits = 1 */
    for (i = 0; i < (unsigned int)len; ++i) {
        if ((m >> i) & (idx_t)1)
            result += pow9[i];
    }
    return result;
}
