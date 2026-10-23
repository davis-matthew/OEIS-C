/*
 * A041926  Numerators of continued fraction convergents to sqrt(486).
 * https://oeis.org/A041926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 970*a(n-2) - a(n-4)
   Initial values: a(0)=22, a(1)=485, a(2)=21362, a(3)=470449 */
#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;
static size_t a_size = 0;
static size_t a_computed = 0;
static const seq_t a_base[4] = { (seq_t)22, (seq_t)485, (seq_t)21362, (seq_t)470449 };

seq_t a(idx_t n)
{
    size_t n_u = (size_t)n;
    if (n_u < 4)
        return a_base[n_u];

    if (!a_cache) {
        a_cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (!a_cache) abort();
        for (size_t i = 0; i < 4; ++i)
            a_cache[i] = a_base[i];
        a_size = 4;
        a_computed = 4; /* first missing index */
    }

    if (n_u >= a_computed) {
        if (n_u + 1 > a_size) {
            size_t new_size = a_size * 2;
            if (new_size < n_u + 1)
                new_size = n_u + 1;
            seq_t *tmp = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
            if (!tmp) abort();
            a_cache = tmp;
            a_size = new_size;
        }
        for (size_t i = a_computed; i <= n_u; ++i) {
            seq_t val = a_cache[i-2] * (seq_t)970 - a_cache[i-4];
            a_cache[i] = val;
        }
        a_computed = n_u + 1;
    }

    return a_cache[n_u];
}
