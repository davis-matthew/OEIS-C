/*
 * A202278  Right-truncatable Fibonacci numbers: every prefix is Fibonacci
 *          number.
 * https://oeis.org/A202278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A202278 (Right‑truncatable Fibonacci numbers).
   The algorithm generates Fibonacci numbers in order, skipping the
   duplicate 1, and stores those whose floor(n/10) is also a Fibonacci
   number.  The precomputed list contains all 11 terms and is reused
   for subsequent calls. */
#include <stdbool.h>
#include <stddef.h>

static bool initialized = false;
static seq_t fib_cache[128];
static size_t fib_len = 0;
static seq_t seq_cache[128];
static size_t seq_len = 0;

/* Return true if val equals any of the fib_cache entries.  */
static bool is_fib_in_cache(seq_t val) {
    for (size_t i = 0; i < fib_len; ++i)
        if (fib_cache[i] == val)
            return true;
    return false;
}

static void init_cache(void) {
    if (initialized)
        return;
    initialized = true;

    /* Seed Fibonacci numbers: F0=0, F1=1. */
    fib_cache[0] = (seq_t)0;
    fib_cache[1] = (seq_t)1;
    fib_len = 2;

    /* Store the first two terms of the sequence (0,1). */
    seq_cache[0] = (seq_t)0;
    seq_cache[1] = (seq_t)1;
    seq_len = 2;

    /* Generate Fibonacci numbers until all 11 terms are found. */
    while (seq_len < 11) {
        /* Next Fibonacci number. */
        seq_t next = fib_cache[fib_len - 1] + fib_cache[fib_len - 2];
        fib_cache[fib_len++] = next;

        /* Skip duplicate 1 that would otherwise appear as a separate term. */
        if (next == fib_cache[fib_len - 2])
            continue;

        /* floor(next / 10) for integer division. */
        seq_t trunc = next / (seq_t)10;

        /* Check if the truncated value is a Fibonacci number. */
        if (is_fib_in_cache(trunc)) {
            seq_cache[seq_len++] = next;
            /* Sequence is known to terminate after 11 terms. */
        }

        /* Safeguard against infinite loops (should never hit). */
        if (fib_len > 200)
            break;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1, so n=0 is undefined */
    init_cache();
    if (n <= seq_len)
        return seq_cache[n - 1];
    return (seq_t)0;               /* no term beyond the 11 finite terms */
}
