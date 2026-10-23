/*
 * A017835  Expansion of 1/(1-x^4-x^5-x^6-x^7-x^8-x^9-x^10-x^11-x^12-x^13).
 * https://oeis.org/A017835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑4) + a(n‑5) + … + a(n‑13), * with initial values *
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=0, * a(4)=1, a(5)=1, a(6)=1,
 *            a(7)=1, * a(8)=2, a(9)=3, a(10)=4, a(11)=5, a(12)=7, a(13)=10 *
 *            (offset 0)
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 76, u64 n <= 146,
 *            u128 n <= 286, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;  /* stored terms */
    static size_t  len    = 0;     /* number of terms actually computed */
    static size_t  cap    = 0;     /* allocated capacity in terms */

    /* ---------- initialisation (first call) ---------- */
    if (len == 0) {
        /* we need to know the first 14 terms */
        len = 14;
        cap = 16;                      /* small over‑provision */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */

        cache[0] = 1;  cache[1] = 0;  cache[2] = 0;  cache[3] = 0;
        cache[4] = 1;  cache[5] = 1;  cache[6] = 1;  cache[7] = 1;
        cache[8] = 2;  cache[9] = 3;  cache[10] = 4; cache[11] = 5;
        cache[12] = 7; cache[13] = 10;
    }

    /* ---------- return cached value if possible ---------- */
    size_t idx = (size_t)n;
    if (idx < len) return cache[idx];

    /* ---------- grow capacity if necessary ---------- */
    if (cap <= idx) {
        size_t new_cap = (cap == 0 ? 16 : cap);
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* ---------- compute missing entries ---------- */
    for (size_t i = len; i <= idx; ++i) {
        seq_t sum = 0;
        /*   a(i) = a(i-4)+a(i-5)+…+a(i-13)   */
        sum += cache[i-4];
        sum += cache[i-5];
        sum += cache[i-6];
        sum += cache[i-7];
        sum += cache[i-8];
        sum += cache[i-9];
        sum += cache[i-10];
        sum += cache[i-11];
        sum += cache[i-12];
        sum += cache[i-13];
        cache[i] = sum;
    }
    len = idx + 1;          /* we now know all terms up to idx */

    return cache[idx];
}
