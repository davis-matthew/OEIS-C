/*
 * A210886  Dates after Jan 01 1000 in chronological order which are
 *          palindromic when they are written in the format DDMMYYYY. Leading
 *          zeros of the terms are suppressed.
 * https://oeis.org/A210886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Generate palindromic dates in DDMMYYYY format after Jan 1 1000
 * Range:     u8 none, u16 none, u32 n <= 335, u64 n <= 335, u128 n <= 335,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) returns the nth such date (n starts at 1). */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *pal_seq = NULL;
static size_t pal_cnt = 0;

static void generate_pal_dates(void)
{
    if (pal_seq) return;
    size_t cap = 400;
    pal_seq = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!pal_seq) exit(1);

    for (int Y = 1000; Y <= 9999; ++Y) {
        int leap = ((Y % 4 == 0 && Y % 100 != 0) || (Y % 400 == 0));
        for (int M = 1; M <= 12; ++M) {
            int mdays;
            switch (M) {
                case 2: mdays = leap ? 29 : 28; break;
                case 4: case 6: case 9: case 11: mdays = 30; break;
                default: mdays = 31; break;
            }
            for (int D = 1; D <= mdays; ++D) {
                char s[9];
                snprintf(s, 9, "%02d%02d%04d", D, M, Y);
                if (s[0] == s[7] && s[1] == s[6] && s[2] == s[5] && s[3] == s[4]) {
                    seq_t v = 0;
                    for (int i = 0; i < 8; ++i)
                        v = v * 10 + (seq_t)(s[i] - '0');
                    if (pal_cnt >= cap) {
                        cap <<= 1;
                        pal_seq = (seq_t *)realloc(pal_seq, cap * sizeof(seq_t));
                        if (!pal_seq) exit(1);
                    }
                    pal_seq[pal_cnt++] = v;
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    generate_pal_dates();
    if ((size_t)(n - 1) >= pal_cnt) return (seq_t)0;
    return pal_seq[n - 1];
}
