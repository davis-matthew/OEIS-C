/*
 * A033644  Base 7 digital convolution sequence.
 * https://oeis.org/A033644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Σ_{i=0}^{k-1} d_i * a(n-i-1) where the digits d_i are the
 *            base‑7 digits of n taken from least significant to most. a(0) =
 *            1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t sz   = 0;
    static size_t cap  = 0;

    /* initialise cache if first call */
    if (sz == 0) {
        if (cap == 0) {
            cap = 1;
            cache = (seq_t*)malloc(cap * sizeof(seq_t));
            if (!cache) exit(1);
        }
        cache[0] = (seq_t)1;
        sz = 1;
    }

    /* grow array if needed */
    if (n + 1 > cap) {
        size_t old_cap = cap;
        size_t new_cap = (n + 1) * 2;
        seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) exit(1);
        cache = newp;
        cap = new_cap;
        /* zero the newly allocated part */
        memset(cache + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
    }

    /* compute new entries up to n */
    if (n + 1 > sz) {
        for (idx_t i = sz; i <= n; ++i) {
            if (i == 0) {            /* already set, but keep for safety */
                cache[0] = (seq_t)1;
                continue;
            }

            /* extract base‑7 digits of i least significant first */
            idx_t temp = i;
            uint8_t digits[32];   /* sufficient for 64‑bit numbers in base 7 */
            int pos = 0;
            do {
                digits[pos++] = temp % 7;
                temp /= 7;
            } while (temp > 0);

            seq_t sum = 0;
            for (idx_t j = 0; j < (idx_t)pos; ++j) {
                seq_t mult = digits[j];
                if (mult == 0) continue;
                sum += mult * cache[i - j - 1];
            }
            cache[i] = sum;
        }
        sz = n + 1;
    }

    return cache[n];
}
