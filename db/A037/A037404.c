/*
 * A037404  Numbers k such that every base-7 digit of k is a base-10 digit of
 *          k.
 * https://oeis.org/A037404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 980, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9850 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A037404: Numbers k such that every base-7 digit of k is a base-10 digit of k.
 * The function enumerates integers in increasing order, maintaining the
 * decimal and base‑7 digit arrays explicitly, to avoid expensive
 * conversions on arbitrary‑precision seq_t values.
 */

#include <stdbool.h>

/* Increment the base‑7 number stored in `base7` (little‑endian digits)
   and recompute its digit counts. */
static void inc_base7(int *base7, int *len, int base7_cnt[7])
{
    int i;
    for (i = 0; i < *len; ++i) {
        if (base7[i] < 6) {
            ++base7[i];
            break;
        }
        base7[i] = 0;
    }
    if (i == *len) {          /* all were 6 -> new most significant digit */
        base7[*len] = 1;
        ++(*len);
    }

    /* recompute counts */
    for (i = 0; i < 7; ++i) base7_cnt[i] = 0;
    for (i = 0; i < *len; ++i) ++base7_cnt[base7[i]];
}

/* Increment the decimal number stored in `dec` (little‑endian digits)
   and recompute its digit counts. */
static void inc_dec(int *dec, int *len, int dec_cnt[10])
{
    int i;
    for (i = 0; i < *len; ++i) {
        if (dec[i] < 9) {
            ++dec[i];
            break;
        }
        dec[i] = 0;
    }
    if (i == *len) {          /* all were 9 -> new most significant digit */
        dec[*len] = 1;
        ++(*len);
    }

    /* recompute counts */
    for (i = 0; i < 10; ++i) dec_cnt[i] = 0;
    for (i = 0; i < *len; ++i) ++dec_cnt[dec[i]];
}

/* Maximum number of decimal or base‑7 digits needed for the
   expected range of n.  Thirty‑two digits is far enough for
   the first 10 000 terms. */
#define MAX_DIG 32

/* Convert little‑endian decimal digits to a seq_t value. */
static seq_t digits_to_seq(const int *dec, int len)
{
    seq_t v = 0;
    for (int i = len - 1; i >= 0; --i) {
        v = v * 10 + dec[i];
    }
    return v;
}

seq_t a(idx_t n)
{
    /* decimal representation of the current candidate (little‑endian) */
    static int dec[MAX_DIG];
    int dec_len = 1;          /* start at 1 */
    dec[0] = 1;
    int dec_cnt[10] = {0};
    dec_cnt[1] = 1;

    /* base‑7 representation of the current candidate (little‑endian) */
    static int base7[MAX_DIG];
    int base7_len = 1;
    base7[0] = 1;
    int base7_cnt[7] = {0};
    base7_cnt[1] = 1;

    int matches = 0;          /* number of terms found so far */

    for (;;) {
        /* Test whether every base‑7 digit appears in the decimal digits */
        bool ok = true;
        for (int d = 0; d <= 6; ++d) {
            if (base7_cnt[d] && !dec_cnt[d]) { ok = false; break; }
        }

        if (ok) {
            ++matches;
            if ((idx_t)matches == n) {
                return digits_to_seq(dec, dec_len);
            }
        }

        /* Advance to the next integer */
        inc_dec(dec, &dec_len, dec_cnt);
        inc_base7(base7, &base7_len, base7_cnt);
    }
}
