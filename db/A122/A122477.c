/*
 * A122477  Numbers k such that k and k^2 together contain all ten digits.
 * https://oeis.org/A122477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2466, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9368 (2026-11-03)
 * Helpers:   digit_frequencies, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A122477: numbers k such that the decimal digits of k and k^2 together contain all ten digits 0–9. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h"
#include "../utils/mem.h"

static seq_t *seq_values = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static seq_t last_checked = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* offset is 1, but guard zero index */

    /* lazy initialization */
    if (seq_values == NULL) {
        seq_cap = 4;
        seq_values = mem_new(seq_t, seq_cap);
        seq_len = 0;
        last_checked = 0;
    }

    /* generate terms until at least n have been found */
    while (seq_len < (size_t)n) {
        last_checked++;                       /* next candidate k */
        seq_t k   = last_checked;
        seq_t ksq = k * k;

        unsigned int freq[10] = {0};
        digit_frequencies(k, freq);

        unsigned int freq2[10];
        digit_frequencies(ksq, freq2);
        for (int d = 0; d < 10; ++d) freq[d] += freq2[d];

        int ok = 1;
        for (int d = 0; d < 10; ++d) {
            if (freq[d] == 0) { ok = 0; break; }
        }
        if (ok) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap * 2;
                seq_t *tmp = mem_resize(seq_t, seq_values, new_cap);
                seq_values = tmp;
                seq_cap = new_cap;
            }
            seq_values[seq_len++] = k;
        }
    }
    return seq_values[n - 1];
}
