/*
 * A199912  Number of -n..n arrays x(0..4) of 5 elements with zero sum, and
 *          adjacent elements not equal modulo three (with -1 modulo 3 = 2).
 * https://oeis.org/A199912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A199912
   Formula (linear recurrence):
   a(n) = a(n‑1) + 4*a(n‑3) – 4*a(n‑4) – 6*a(n‑6) + 6*a(n‑7)
          + 4*a(n‑9) – 4*a(n‑10) – a(n‑12) + a(n‑13)
   Initial values (n = 1…13):
   a(1)=14, a(2)=82, a(3)=256, a(4)=804, a(5)=1836, a(6)=3196,
   a(7)=6064, a(8)=10276, a(9)=14846, a(10)=23154, a(11)=34096,
   a(12)=44912, a(13)=63114
   The recurrence is applied left‑to‑right, so unsigned arithmetic is safe. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence values for n = 1..13 (index 0 unused) */
    static const seq_t base_vals[14] = {
        0,
        (seq_t)14,  (seq_t)82,  (seq_t)256,  (seq_t)804,
        (seq_t)1836, (seq_t)3196, (seq_t)6064, (seq_t)10276,
        (seq_t)14846, (seq_t)23154, (seq_t)34096, (seq_t)44912,
        (seq_t)63114
    };

    /* Cached values */
    static seq_t *cache = NULL;      /* pointer to array of seq_t */
    static idx_t  cached_upto = 0;   /* largest n for which cache[n] is valid */

    if (n == 0) {
        return (seq_t)0; /* by convention, outside the offset */
    }

    /* Resize cache if needed */
    if (n > cached_upto) {
        idx_t new_cap = n + 1;               /* indices 0..n inclusive */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory – in this context we simply abort */
            abort();
        }
        cache = new_cache;

        /* Initialise entries up to min(n,13) with known base values */
        for (idx_t i = cached_upto + 1; i <= n && i <= 13; ++i) {
            cache[i] = base_vals[i];
        }

        /* Compute new values using the recurrence */
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            if (i <= 13) {
                /* already set */
                continue;
            }
            seq_t val = cache[i - 1];
            val += 4 * cache[i - 3];
            val -= 4 * cache[i - 4];
            val -= 6 * cache[i - 6];
            val += 6 * cache[i - 7];
            val += 4 * cache[i - 9];
            val -= 4 * cache[i - 10];
            val -=       cache[i - 12];
            val +=       cache[i - 13];
            cache[i] = val;
        }
        cached_upto = n;
    }

    return cache[n];
}
