/*
 * A129525  Numbers k such that all the digits of k^3 are distinct.
 * https://oeis.org/A129525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth positive integer k such that all digits of k³ are distinct.
 * The sequence is finite because a cube with more than 10 digits must contain a repeated
 * digit.  It suffices to search k up to 2154 (since 2154³ has 10 digits).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of found k values */
    static seq_t *cache = NULL;
    static size_t len = 0;
    const size_t MAX_K = 2154;          /* cubes up to 10 digits */

    /* initialise cache once */
    if (len == 0) {
        /* Allocate more space than needed; we will fill only `len` entries. */
        cache = (seq_t*)malloc((MAX_K + 1) * sizeof(seq_t));
        if (!cache) return 0;          /* defensive: out of memory */

        size_t count = 0;
        for (idx_t k = 1; k <= MAX_K; ++k) {
            unsigned long long cube = (unsigned long long)k * k * k;
            bool seen[10] = { false };
            bool ok = true;

            unsigned long long tmp = cube;
            /* Extract digits from least‑significant to most‑significant */
            do {
                unsigned d = (unsigned)(tmp % 10);
                if (seen[d]) { ok = false; break; }
                seen[d] = true;
                tmp /= 10;
            } while (tmp);

            if (ok) {
                cache[count++] = (seq_t)k;
            }
        }
        len = count;    /* number of valid terms stored */
    }

    /* bounds check: if n falls outside the known terms, return 0 */
    if (n == 0 || n > (idx_t)len) {
        return 0;
    }
    return cache[(size_t)(n - 1)];
}
