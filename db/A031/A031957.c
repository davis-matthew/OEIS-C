/*
 * A031957  Numbers with exactly three distinct base-5 digits.
 * https://oeis.org/A031957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A031957 – Numbers with exactly three distinct base‑5 digits
   Computed by brute‑force enumeration. */
#include <stdlib.h>

static seq_t *cache = NULL;       /* cached sequence values   */
static size_t cap = 0;            /* current allocation size  */
static size_t stored = 0;         /* number of values stored  */
static seq_t last_checked = 0;    /* largest number examined */

static void grow_cache(void) {
    size_t new_cap = cap ? cap * 2 : 128;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset 1 – undefined for n≤0 */

    /* Generate until we have at least n terms */
    if (stored < n) {
        seq_t cur = last_checked + 1;
        while (stored < n) {
            seq_t t = cur;
            unsigned mask = 0;
            while (t > 0) {
                unsigned d = (unsigned)(t % 5);
                mask |= 1u << d;
                t /= 5;
            }
            /* count bits set in mask */
            unsigned cnt = 0;
            unsigned m = mask;
            while (m) { cnt += m & 1u; m >>= 1; }

            if (cnt == 3) {
                if (stored == cap) grow_cache();
                cache[stored++] = cur;
            }
            ++cur;
        }
        last_checked = cur - 1;
    }
    return cache[n - 1];
}
