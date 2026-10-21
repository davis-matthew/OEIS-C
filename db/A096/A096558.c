/*
 * A096558  Output of the linear congruential pseudo-random number generator
 *          rand() used in Microsoft's Visual C++.
 * https://oeis.org/A096558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (( (b(n-1)*214013 + 2531011) mod 2^32 ) >> 16) & 0x7FFF
 *            with b(1) = 0, a(n) = floor(b(n)/65536) mod 32768
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n) {
    /* static cache for already computed terms */
    static seq_t *cache = NULL;      /* array of a values */
    static size_t cap = 0;           /* allocated capacity */
    static size_t len = 0;           /* number of terms computed */
    static uint32_t last_b = 0;      /* b value of the last computed index */

    if (n == 0) return (seq_t)0;     /* safety: undefined for n<1 */
    /* compute up to n if necessary */
    while (len < (size_t)n) {
        if (cap <= len) {
            size_t newcap = cap ? cap << 1 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) exit(1);      /* allocation failure */
            cache = tmp;
            cap = newcap;
        }
        /* compute a for index len+1 */
        if ((size_t)len + 1 == 1) {
            /* b(1) is already 0 in last_b */
        } else {
            last_b = last_b * 214013u + 2531011u;  /* 32‑bit overflow */
        }
        cache[len++] = (seq_t)((last_b >> 16) & 0x7FFFu);
    }
    return cache[n - 1];
}
