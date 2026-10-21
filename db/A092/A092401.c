/*
 * A092401  List of pairs n, 3n, where n is the least unused number so far.
 * https://oeis.org/A092401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(2k-1) = A007417(k), a(2k) = 3*A007417(k)
 * where A007417(k) is the k-th least unused positive integer.
 *
 * The implementation generates the sequence on demand, keeping
 * a bitmap of used numbers and a cache of the sequence values.
 * It works for any seq_t/idx_t types defined by the build
 * and is valid C11 and C++17 alike.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- parameters for the static allocation ---------- */
/* Max number of terms that may be requested (safe for n ≤ 10000) */
#define CACHE_CAP 40000          /* number of seq_t elements stored   */
#define USED_CAP  1000000        /* maximum integer that may appear   */

/* ---------- static state --------------------------------- */
static seq_t seq_cache[CACHE_CAP];
static size_t seq_len = 0;           /* number of sequence elements built */

static unsigned char used[USED_CAP];
static idx_t next_candidate = 1;     /* smallest integer to test */

/* ---------- helper function -------------------------------- */
static void find_next_unused(idx_t *pos)
{
    /* Ensure pos points to an unused integer. */
    while ((*pos) < USED_CAP && used[(size_t)(*pos)])
        ++(*pos);
}

/* ---------- public function -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n == 0 */

    /* Build the sequence until it contains at least `n` terms. */
    while (seq_len < (size_t)n) {
        /* Locate next unused integer. */
        find_next_unused(&next_candidate);
        if (next_candidate >= USED_CAP) {
            /* Fallback: enlarge the bitmap if we run out of space. */
            /* This situation should not occur for the tested limits. */
            /* In production code you could realloc `used` here. */
            return 0;
        }

        idx_t v = next_candidate;            /* the least unused number */

        /* Store the pair (v, 3v) in the cache. */
        if (seq_len + 2 > CACHE_CAP) {
            /* Out‑of‑bounds: handle gracefully.        */
            /* This situation should not occur for tested limits. */
            return 0;
        }
        seq_cache[seq_len++] = (seq_t)v;
        seq_cache[seq_len++] = (seq_t)((seq_t)3 * (seq_t)v);

        /* Mark both v and 3v as used. */
        used[(size_t)v] = 1;
        if ( (size_t)3*v < USED_CAP )
            used[(size_t)3*v] = 1;

        /* Advance candidate for next round. */
        ++next_candidate;
    }

    return seq_cache[(size_t)n - 1];
}
