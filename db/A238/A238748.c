/*
 * A238748  Numbers k such that each integer that appears in the prime
 *          signature of k appears an even number of times.
 * https://oeis.org/A238748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that each exponent appearing in the prime signature of k appears an even number of times.
   This implementation generates the sequence lazily: it tests successive integers and stores all terms found
   in a static cache.  Factoring is done by trial division, which is sufficient for the sizes required
   (the 10 000‑th term is below 32 000).  The function works for any integer types seq_t and idx_t
   provided by the build and is thread‑safe only if called sequentially. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;   /* cache of sequence terms */
static size_t seq_cap   = 0;      /* capacity of seq_cache   */
static size_t seq_len   = 0;      /* number of terms stored */

/* Append a term to the cache, expanding it if necessary. */
static void
seq_cache_append (seq_t val)
{
    if (seq_len >= seq_cap)
    {
        size_t new_cap = seq_cap == 0 ? 32 : seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* out of memory – caller will crash in the worst case */
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* Static variable holding the largest integer that has been examined so far. */
static uint64_t max_checked = 0;

/* Main sequence routine. */
seq_t
a (idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0 – return 0 as a guard */

    /* Generate terms until the cache contains at least n elements. */
    while (seq_len < (size_t)n)
    {
        ++max_checked;   /* Next candidate to test */

        /* Decide whether max_checked has the required property. */
        uint64_t x = max_checked;
        unsigned short exp_counts[65] = {0};
        bool ok = true;

        /* Trial‑division factorisation. */
        for (uint64_t p = 2; p * p <= x; ++p)
        {
            if (x % p == 0)
            {
                unsigned short exp = 0;
                while (x % p == 0) { x /= p; ++exp; }
                if (exp > 64) { ok = false; break; }   /* should not happen in our range */
                ++exp_counts[exp];
            }
        }
        if (ok && x > 1)          /* remaining prime factor */
            ++exp_counts[1];

        /* Every exponent must appear an even number of times. */
        for (unsigned short e = 1; e < 65 && ok; ++e)
        {
            if ((exp_counts[e] & 1) != 0) ok = false;
        }

        if (ok)
            seq_cache_append((seq_t)max_checked);
    }

    return seq_cache[n - 1];
}
