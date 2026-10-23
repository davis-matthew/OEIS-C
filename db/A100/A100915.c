/*
 * A100915  Numbers n such that n plus n-th semiprime is semiprime.
 * https://oeis.org/A100915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n such that n + s(n) is semiprime, where s(n) is the n‑th
 *            semiprime and a(n) enumerates the n‑th such number. The sequence
 *            is indexed from 1
 * Range:     u8 n <= 68, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, grow_uint64_array, is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_semiprime_uint64.h"
#include "../utils/grow_seq_array.h"
#include "../utils/grow_uint64_array.h"

/* --- semiprime sequence ---------------------------------------------- */
static uint64_t *sp_cache = NULL;
static size_t     sp_cap   = 0;
static size_t     sp_size  = 0;
static uint64_t   sp_next  = 2;          /* next candidate to test */

/* obtain the next semiprime in ascending order */
static uint64_t next_semiprime(void)
{
    while (!is_semiprime_uint64(sp_next)) ++sp_next;
    uint64_t val = sp_next;
    ++sp_next;
    return val;
}

/* ensure sp_cache contains semiprimes 1..idx (inclusive) */
static void ensure_semiprimes(size_t idx)
{
    if (sp_size >= idx) return;
    grow_uint64_array(&sp_cache, &sp_cap, idx);
    while (sp_size < idx) sp_cache[sp_size++] = next_semiprime();
}

/* --- sequence a(n) ----------------------------------------------- */
static seq_t   *a_cache   = NULL;
static size_t   a_cap     = 0;
static size_t   a_size    = 0;
static idx_t    next_candidate_n = 0; /* last n tested for a(n) */

/* ensure that a_cache contains the first n terms of the sequence */
static void ensure_sequence(size_t n_needed)
{
    if (a_size >= n_needed) return;

    grow_seq_array(&a_cache, &a_cap, n_needed);

    while (a_size < n_needed) {
        ++next_candidate_n;            /* candidate n to test */

        /* obtain s(n) */
        ensure_semiprimes((size_t)next_candidate_n);
        uint64_t s = sp_cache[next_candidate_n - 1];

        /* test if n + s(n) is semiprime */
        if (is_semiprime_uint64((uint64_t)next_candidate_n + s)) {
            a_cache[a_size++] = (seq_t)next_candidate_n;
        }
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */
    ensure_sequence((size_t)n);
    return a_cache[n - 1];
}
