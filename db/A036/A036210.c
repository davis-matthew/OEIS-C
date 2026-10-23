/*
 * A036210  Log base 3 (n) mod 257.
 * https://oeis.org/A036210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 256, u32 n <= 256, u64 n <= 256,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence A036210: a(n) = discrete logarithm base 3 of (n mod 257)
 * modulo 257.  A precomputed table of exponents 0..255 is built once and used
 * for all queries.  The table maps every non‑zero residue r (1 ≤ r < 257)
 * to the unique k with 3^k ≡ r (mod 257).  For n divisible by 257 the value
 * 0 is returned (the logarithm is undefined).  The computation is O(1)
 * per call after an O(256) one‑time initialization. */

#include <stdint.h>
#include <stdbool.h>

static seq_t log3_table[257];
static bool log3_initialized = false;

static void build_log3_table(void)
{
    uint64_t cur = 1;          /* 3^0 mod 257 */
    for (uint64_t k = 0; k < 256; ++k)
    {
        log3_table[cur] = (seq_t)k;    /* store exponent k for residue cur */
        cur = (cur * 3ULL) % 257ULL;   /* next residue */
    }
}

seq_t a(idx_t n)
{
    if (!log3_initialized) {
        build_log3_table();
        log3_initialized = true;
    }

    idx_t r = n % 257;            /* reduce n modulo 257 */
    if (r == 0)                  /* undefined for multiples of 257 */
        return (seq_t)0;

    return log3_table[r];         /* discrete log of r modulo 257 */
}
