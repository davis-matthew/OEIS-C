/*
 * A037403  Numbers k such that every base-7 digit of k is a base-9 digit of
 *          k.
 * https://oeis.org/A037403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1298, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find numbers k such that every base‑7 digit of k also appears as a base‑9
   digit of k.  The function generates the sequence on demand, storing
   results in a static cache so that calls with arbitrary indices are fast. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* guard for the offset: we do not define a(0) */
    if (n == 0) return 0;

    /* static cache of known terms */
    static seq_t *cache = NULL;
    static idx_t count = 0;   /* number of terms stored so far      */
    static idx_t capacity = 0;/* allocated capacity of the cache      */
    static seq_t last_tested = 1;/* next integer to test (initialized to 1) */

    /* grow the cache if it cannot hold the required number of terms */
    if (cache == NULL || capacity < count + n) {
        idx_t newcap = capacity == 0 ? 64 : capacity * 2;
        while (newcap < count + n) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* allocation failed */
        cache = tmp;
        capacity = newcap;
    }

    /* generate new terms until we have at least n of them */
    while (count < n) {
        seq_t test = last_tested; /* start from the next integer after last found */

        for (;;) {
            unsigned int mask7 = 0, mask9 = 0;
            seq_t t = test;

            /* build the digit masks for base 7 and base 9 */
            while (t != 0) {
                mask7 |= 1U << (unsigned int)(t % 7);
                t /= 7;
            }
            t = test;
            while (t != 0) {
                mask9 |= 1U << (unsigned int)(t % 9);
                t /= 9;
            }

            /* check if every base‑7 digit appears in the base‑9 representation */
            if ((mask7 & ~mask9) == 0) {
                cache[count] = test;
                count++;
                last_tested = test + 1;  /* next candidate after this one */
                break;
            }
            test++;
        }
    }

    return cache[n - 1];
}
