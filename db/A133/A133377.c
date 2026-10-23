/*
 * A133377  Complete list of decimal numbers that when converted to
 *          hexadecimal produce the mirror image of the original number.
 * https://oeis.org/A133377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, nonn, full
 * Formula:   a(n) = numbers n such that hex(n) uses only digits 0‑9 and
 *            hex(n) equals reverse(decimal(n))
 * Range:     u8 none, u16 none, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static bool initialized = false;

static void init_cache(void)
{
    if (initialized) return;
    initialized = true;

    /* All known terms are below 100 000; we search a bit beyond for safety. */
    const idx_t LIMIT = 200000;

    size_t capacity = 64;
    cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
    if (!cache) return; /* allocation failure – we simply give up */

    cache_len = 0;

    for (seq_t n = 0; n <= LIMIT; ++n) {
        seq_t tmp = n;
        int hex[20];
        size_t hex_len = 0;
        bool ok = true;

        if (tmp == 0) {          /* zero is a valid term */
            if (cache_len == capacity) {
                capacity <<= 1;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * capacity);
            }
            cache[cache_len++] = n;
            continue;
        }

        while (tmp > 0) {
            int d = (int)(tmp % 16);
            if (d > 9) { ok = false; break; }  /* hex digit not a decimal digit */
            hex[hex_len++] = d;
            tmp /= 16;
        }
        if (!ok) continue;

        /* decimal digits, least‑to‑most */
        seq_t tmp2 = n;
        int dec[20];
        size_t dec_len = 0;
        while (tmp2 > 0) {
            dec[dec_len++] = (int)(tmp2 % 10);
            tmp2 /= 10;
        }

        if (hex_len != dec_len) continue;

        bool match = true;
        /* compare hex[msd] with dec[lsd] (i.e. reverse of decimal digits) */
        for (size_t i = 0; i < hex_len; ++i) {
            if (hex[hex_len - 1 - i] != dec[i]) {
                match = false;
                break;
            }
        }
        if (!match) continue;

        if (cache_len == capacity) {
            capacity <<= 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * capacity);
        }
        cache[cache_len++] = n;
    }
}

seq_t a(idx_t n)
{
    init_cache();
    if (n == 0 || n > cache_len) return 0;
    return cache[n - 1];
}
