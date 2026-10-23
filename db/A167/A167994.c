/*
 * A167994  Numbers k with the property that the first digit of 2*k is the
 *          last digit of k.
 * https://oeis.org/A167994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, grow_seq, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167994: numbers k such that the first digit of 2*k equals the last digit of k. */

#include "../utils/grow_seq.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* cached sequence data */
    static seq_t *cache = NULL;     /* array holding found terms */
    static size_t cap   = 0;        /* allocated capacity */
    static size_t len   = 0;        /* number of terms stored */
    static seq_t next_k = 1;        /* next k to test */

    /* Generate terms until we have at least n of them. */
    while (len < n)
    {
        seq_t k   = next_k++;          /* candidate k */
        seq_t last = k % 10;           /* last decimal digit of k */

        seq_t twice = k + k;           /* 2*k */
        int  dlen   = digit_len(twice);/* number of decimal digits of 2*k */
        seq_t p10   = pow10((idx_t)(dlen - 1)); /* 10^(dlen-1) */
        seq_t first = twice / p10;     /* first decimal digit of 2*k */

        if (first == last)            /* property holds */
        {
            grow_seq(&cache, &cap, len + 1);
            cache[len++] = k;
        }
    }

    return cache[n - 1];
}
