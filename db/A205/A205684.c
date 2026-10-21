/*
 * A205684  Numbers k for which 5 divides prime(k)-prime(j) for some j<k; each
 *          k occurs once for each such j.
 * https://oeis.org/A205684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A205684
    The k-th output is the index of a prime p_k such that 5 | (p_k – p_j)
    for some earlier j < k.  Each such j produces a separate occurrence
    of k.  Equivalently, for each prime index k, let r = p_k mod 5.
    Let count_r be the number of previous primes with residue r;
    then k appears count_r times in the sequence.  We generate the
    sequence on demand and store it in a static array.  This is
    O(n) in the number of terms generated (about 0.1 k^2 for k terms,
    so about 3×10^5 terms for n≈10^5).  The function may be called
    in any order; the static state keeps the cache up to the
    requested index. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"

static seq_t *seq_cache = NULL;        /* sequence terms (k values) */
static size_t seq_len   = 0;            /* number of stored terms */
static size_t seq_cap   = 0;            /* capacity of seq_cache */

static unsigned mod_counts[5] = {0};   /* number of primes seen per residue modulo 5 */
static idx_t next_prime_idx = 1;       /* next prime index to process */

/* Ensure the cache can hold at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 32;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* Append a single term to the cache */
static void append_value(seq_t val)
{
    ensure_cache_cap(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* Expand the cache until it contains at least 'target' terms */
static void extend_cache_to(size_t target)
{
    while (seq_len < target) {
        uint32_t p = get_prime(next_prime_idx);      /* k-th prime */
        unsigned r = p % 5U;                         /* residue modulo 5 */
        unsigned cnt = mod_counts[r];                /* how many earlier primes share this residue */

        seq_t val = (seq_t)next_prime_idx;           /* the index k to output */

        for (unsigned i = 0; i < cnt; ++i)
            append_value(val);                       /* each earlier j yields an occurrence of k */

        mod_counts[r] = cnt + 1;                     /* one more prime of this residue encountered */
        ++next_prime_idx;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence is 1‑based; a(0) is undefined */
    extend_cache_to((size_t)n);               /* ensure cache holds at least n terms */
    return seq_cache[n - 1];
}
