/*
 * A145254  a(n) = the smallest positive integer that is non-coprime to n and
 *          has the same number of 1's in its binary representation as n.
 * https://oeis.org/A145254
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = min{m>0 : gcd(m,n)>1 and popcount(m)=popcount(n)} -
 *            brute‑force search
 * Range:     u8 n <= 255, u16 n <= 9612, u32 n <= 9612, u64 n <= 9612,
 *            u128 n <= 9612, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* popcount for seq_t up to 128 bits. */
static unsigned popcount_seq_t(seq_t x)
{
    unsigned c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

static seq_t *g_cache = NULL;
static idx_t g_cap = 0;

static void ensure_cache(idx_t n)
{
    if (n + 1 > g_cap) {
        idx_t new_cap = g_cap ? (g_cap << 1) : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* memory exhaustion */
        if (new_cap > g_cap) {
            for (idx_t i = g_cap; i < new_cap; ++i)
                tmp[i] = 0;
        }
        g_cache = tmp;
        g_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined for n < 2 */

    ensure_cache(n);
    if (g_cache[n])          /* non‑zero means cached */
        return g_cache[n];

    seq_t nn = (seq_t)n;
    unsigned k = popcount_seq_t(nn);
    seq_t m = 2;             /* start at 2 (smallest positive integer >0) */

    while (true) {
        if (popcount_seq_t(m) == k && gcd(m, nn) != (seq_t)1)
            break;
        ++m;
    }

    g_cache[n] = m;
    return m;
}
