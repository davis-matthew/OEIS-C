/*
 * A154874  Numbers k such that k^3 contains every digit exactly twice.
 * https://oeis.org/A154874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 138, u64 n <= 138, u128 n <= 138,
 *            big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154874: Numbers k such that k³ contains each digit 0–9 exactly twice. */
/* Implementation: search k from the first known term, compute k³ in an
 * unsigned 128‑bit integer, count the decimal digits, and collect the k's
 * that satisfy the condition.  The search stops after 138 values are
 * found (the known size of the sequence). */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/append_candidate.h"

static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static bool  initialized = false;

static void init_cache(void)
{
    if (initialized) return;

    const idx_t MIN_K = 2158479;        /* first term of the sequence   */
    const int   TARGET = 138;           /* total number of terms known */
    unsigned char freq[10];

    for (idx_t k = MIN_K; ; ++k) {
        /* Compute k³ in unsigned 128‑bit to avoid overflow. */
        unsigned __int128 cube = (unsigned __int128)k * k * k;

        /* Extract decimal digits of cube. */
        unsigned char digits[40];
        int dlen = 0;
        unsigned __int128 tmp = cube;
        if (tmp == 0) {
            digits[dlen++] = 0;
        } else {
            while (tmp > 0) {
                digits[dlen++] = (unsigned char)(tmp % 10);
                tmp /= 10;
            }
        }

        /* Count occurrences of each digit. */
        memset(freq, 0, sizeof(freq));
        for (int i = 0; i < dlen; ++i) {
            ++freq[digits[i]];
        }

        /* Check the property. */
        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (freq[d] != 2) { ok = false; break; }
        }

        if (ok) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)k);
            if (cache_len >= (size_t)TARGET) break;
        }

        /* Safety guard: stop if an excessively large k is reached.  */
        if (k > 20000000ULL) break;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init_cache();
    if (n == 0 || (size_t)n > cache_len) return 0;
    return cache[n - 1];
}
