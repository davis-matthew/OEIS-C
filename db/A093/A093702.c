/*
 * A093702  a(n) = smallest prime p>a(n-1) such that n divides (p-1) and
 *          (p-1)/n > (a(n-1)-1)/(n-1), a(1) = 2.
 * https://oeis.org/A093702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p > a(n‑1) such that n divides (p‑1) and
 *            (p‑1)/n > (a(n‑1)‑1)/(n‑1), with a(1) = 2. The implementation
 *            builds the sequence incrementally. Each term a(n) satisfies p =
 *            1 (mod n) and p = 1 + n·k with k > (a(n‑1)‑1)/(n‑1)
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/isPrime.h"

static seq_t *a_vals     = NULL;   /* array of computed a(n) values */
static size_t  computed_max = 0;   /* how many terms are currently stored */
static seq_t  last_a = 0;          /* a(n-1) during construction */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n <= 0 */

    /* extend the stored values up to the requested index n */
    if ((size_t)n > computed_max) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(a_vals, new_size * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failed; abort */
        a_vals = tmp;

        /* compute new terms */
        for (size_t i = computed_max; i < new_size; ++i) {
            idx_t idx = (idx_t)(i + 1);   /* current n */

            if (i == 0) {                 /* a(1) = 2 */
                a_vals[0] = (seq_t)2;
                last_a = (seq_t)2;
                continue;
            }

            /* b_{n-1} = (a(n-1)-1)/(n-1)  (exact division) */
            seq_t b_prev = (last_a - 1) / ((seq_t)(idx - 1));

            /* start with k = b_prev + 1 and find first prime p = 1 + n·k */
            seq_t k = b_prev + 1;
            seq_t p;
            while (1) {
                p = idx * k + 1;                 /* p = 1 + n·k */
                if (isPrime((uint64_t)p)) {      /* primality test */
                    a_vals[i] = p;
                    last_a = p;
                    break;
                }
                ++k;
            }
        }
        computed_max = new_size;
    }

    return a_vals[n - 1];
}
