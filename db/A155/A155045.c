/*
 * A155045  Primes with distinct odd digits.
 * https://oeis.org/A155045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 74, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: full, pass, 88 terms through n = 88 (2026-11-02)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes having all distinct odd decimal digits.
 * The sequence is searched by inspecting all integers up to
 * 99999 – the largest integer that can contain only odd digits.
 * The first `count` such primes are cached; subsequent calls reuse
 * the cache. */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

/* Return true if v has only odd digits and all those digits are unique. */
static bool distinct_odd_digits(uint64_t v)
{
    uint8_t used[10] = {0};
    while (v != 0) {
        uint8_t d = (uint8_t)(v % 10);
        if ((d & 1) == 0)           /* even digit */
            return false;
        if (used[d])
            return false;           /* repeated digit */
        used[d] = 1;
        v /= 10;
    }
    return true;
}

/* External linkage as required by the build. */
seq_t a(idx_t n)
{
    /* Static cache for the prime values. */
    static seq_t cache[500];
    /* Number of primes stored. */
    static idx_t count;
    /* Flag to ensure the cache is built only once. */
    static bool ready;

    if (!ready) {
        count = 0;
        /* Scan all integers up to 99999 – no larger integer can have
         * all odd digits. */
        for (uint64_t val = 3; val <= 99999 && count < 500; ++val) {
            if (distinct_odd_digits(val) && is_prime_seq(val)) {
                cache[count++] = (seq_t)val;
            }
        }
        ready = true;
    }

    if (n == 0 || n > count)
        return 0;          /* undefined for out‑of‑range indices */
    return cache[n - 1];   /* offset 1: a(1) is the first term */
}
