/*
 * A035335  a(n) = smallest number m such that n appears in decimal expansion
 *          of 1/m.
 * https://oeis.org/A035335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 248, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A035335: a(n) = smallest m such that n occurs in the decimal expansion of 1/m */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert the index n into its decimal digit pattern */
    unsigned char pat[32];
    size_t plen = 0;
    idx_t tmp = n;
    do {
        pat[plen++] = '0' + (tmp % 10);
        tmp /= 10;
    } while (tmp);
    /* reverse to normal order */
    for (size_t i = 0; i + i < plen; ++i) {
        unsigned char c = pat[i];
        pat[i] = pat[plen - 1 - i];
        pat[plen - 1 - i] = c;
    }

    /* Search denominators m in increasing order */
    for (idx_t m = 1; ; ++m) {
        /* Remainder table for cycle detection (size m). */
        int *rem_seen = (int*)calloc((size_t)m, sizeof(int));
        if (!rem_seen) return 0;   /* out of memory */

        /* Dynamically grow digit buffer */
        size_t cap = 128;
        size_t len = 0;
        char *digits = (char*)malloc(cap);
        if (!digits) { free(rem_seen); return 0; }

        idx_t r = 1 % m;      /* current remainder after the decimal point */
        size_t cycle_start = 0;
        size_t cycle_len   = 0;
        bool finished = false;

        /* Produce decimal digits of 1/m */
        while (!finished) {
            if (r == 0) {            /* terminating decimal */
                finished = true;
                break;
            }
            if (rem_seen[(size_t)r]) {
                cycle_start = rem_seen[(size_t)r] - 1;
                cycle_len   = len - cycle_start;
                finished = true;
                break;
            }
            rem_seen[(size_t)r] = (int)(len + 1);

            unsigned __int128 tmpu = (unsigned __int128)r * 10;
            unsigned int digit = (unsigned int)(tmpu / m);
            r = (idx_t)(tmpu % m);

            if (len == cap) {
                cap <<= 1;
                char *tmp = (char*)realloc(digits, cap);
                if (!tmp) { free(digits); free(rem_seen); return 0; }
                digits = tmp;
            }
            digits[len++] = (char)('0' + digit);
        }

        bool found = false;

        /* Search for pattern in the generated digits */
        if (finished && r == 0) {     /* terminating */
            for (size_t i = 0; i < len && !found; ++i) {
                size_t k;
                for (k = 0; k < plen; ++k) {
                    char d = (i + k < len) ? digits[i + k] : '0';
                    if (d != pat[k]) break;
                }
                if (k == plen) found = true;
            }
        } else {                     /* repeating */
            /* Build an extended buffer long enough for one more cycle */
            size_t needed = cycle_start + cycle_len + plen;
            if (len < needed) {
                size_t extra = needed - len;
                for (size_t i = 0; i < extra; ++i) {
                    if (len == cap) {
                        cap <<= 1;
                        char *tmp = (char*)realloc(digits, cap);
                        if (!tmp) { free(digits); free(rem_seen); return 0; }
                        digits = tmp;
                    }
                    digits[len++] = digits[cycle_start + (i % cycle_len)];
                }
            }
            for (size_t i = 0; i + plen <= len && !found; ++i) {
                if (memcmp(&digits[i], pat, plen) == 0) {
                    found = true;
                }
            }
        }

        free(digits);
        free(rem_seen);

        if (found) return (seq_t)m;
    }
}
