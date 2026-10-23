/*
 * A044041  Numbers k such that 0 and 1 occur juxtaposed in the base-11
 *          representation of k but not of k+1.
 * https://oeis.org/A044041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   one, zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044041:
   k such that the base‑11 representation of k contains adjacent digits 0 and 1,
   while the representation of k+1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/one.h"
#include "../utils/zero.h"

#ifdef __cplusplus

#else

#endif

/* Check whether the base‑11 expansion of x contains “01” or “10”. */
static bool has_adjacent01_10(seq_t x)
{
    seq_t prev = zero();
    bool first = true;
    while (x > zero()) {
        seq_t d = x % (seq_t)11;
        if (!first) {
            if ((prev == zero() && d == one()) ||
                (prev == one()  && d == zero())) {
                return true;
            }
        }
        prev = d;
        first = false;
        x /= (seq_t)11;
    }
    return false;
}

/* Resize the cache array to at least `needed` elements, keeping `cnt` items. */
static void ensure_capacity(size_t *cap, seq_t **ptr, size_t needed, size_t cnt)
{
    if (*cap >= needed) return;
#ifdef __cplusplus
    seq_t *newptr = new seq_t[needed];
    for (size_t i = 0; i < cnt; ++i) newptr[i] = (*ptr)[i];
    delete[] *ptr;
    *ptr = newptr;
#else
    seq_t *newptr = realloc(*ptr, needed * sizeof(seq_t));
    if (!newptr) exit(1);
    *ptr = newptr;
#endif
    *cap = needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return zero();

    static seq_t *cache = NULL;
    static size_t cnt  = 0;
    static size_t cap  = 0;

    if (cnt >= (size_t)n) return cache[n-1];

    /* Expand cache if needed. */
    ensure_capacity(&cap, &cache, (size_t)n, cnt);

    seq_t start = (cnt == 0) ? one() : cache[cnt-1] + one();
    seq_t k = start;
    for (; cnt < (size_t)n; ++k) {
        if (has_adjacent01_10(k) && !has_adjacent01_10(k + one())) {
            cache[cnt++] = k;
        }
    }
    return cache[n-1];
}
