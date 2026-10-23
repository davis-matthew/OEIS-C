/*
 * A044682  Numbers n such that string 5,5 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the base‑9 representation of n contains the
 *            consecutive digits “55” but the base‑9 representation of n+1
 *            does NOT contain “55”
 * Range:     u8 n <= 3, u16 n <= 801, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Helper to test if a base‑9 number has the substring 55. */
static bool has55(seq_t x)
{
    seq_t prev = 9;            /* sentinel value outside 0‑8 range */
    while (x != 0) {
        seq_t d = x % 9;
        if (d == 5 && prev == 5)
            return true;
        prev = d;
        x /= 9;
    }
    return false;
}

/* Cache of previously generated terms.  The sequence grows slowly
   (≈1 term per 81 numbers), so a static array of 100000 entries
   comfortably covers the expected limits of the tests. */
#define CACHE_CAP 100000
static seq_t cache[CACHE_CAP];
static idx_t cache_size = 0;
static seq_t next_candidate = 0;

/* Public function.  For n ≥ 1 it returns the n‑th term of A044682. */
seq_t a(idx_t n)
{
    /* Guard against a zero index – not required by the specification,
       but harmless. */
    if (n == 0)
        return 0;

    /* Generate terms up to the requested position. */
    while (cache_size < n) {
        if (has55(next_candidate) && !has55(next_candidate + 1)) {
            if (cache_size < CACHE_CAP)
                cache[cache_size++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
