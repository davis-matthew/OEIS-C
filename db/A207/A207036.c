/*
 * A207036  Numbers k such that the sum of divisors of k is a multiple of 24.
 * https://oeis.org/A207036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_mod24
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207036: Numbers k such that the sum of divisors of k is a
   multiple of 24.  The sequence is computed by brute‑force search
   testing each integer k in order and recording those for which
   σ(k) ≡ 0 (mod 24).  The search is memoised so that calls to a(n)
   are answered in O(1) after the range has been generated.  */

/* include basic headers – only <stdlib.h> is strictly needed,
   but <stdint.h> may be required for UINT64_MAX in some
   assembler contexts. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sigma_mod24.h"

/* Memoised storage of the sequence terms. */
static seq_t *s_cache = NULL;
static size_t s_len   = 0;   /* number of stored terms */
static size_t s_cap   = 0;   /* allocated capacity */
static uint64_t next_k = 1;  /* next integer to test */

/* ensure sufficient capacity for at least need elements */
static void ensure_capacity(size_t need)
{
    if (s_cap >= need)
        return;
    size_t newcap = s_cap ? s_cap * 2 : 8;
    if (newcap < need)
        newcap = need;
    seq_t *newptr = (seq_t *)realloc(s_cache, newcap * sizeof(seq_t));
    if (!newptr) abort();   /* out of memory – abort is acceptable here */
    s_cache = newptr;
    s_cap   = newcap;
}

/* public function: nth term of A207036 */
seq_t a(idx_t n)
{
    if (n == 0)  /* Not part of the OEIS offset – return 0 for safety. */
        return 0;

    /* generate terms until we have at least n of them */
    while (s_len < n) {
        /* test current value of next_k for σ(k) ≡ 0 mod 24 */
        if (sigma_mod24((uint64_t)next_k) == 0) {
            ensure_capacity(s_len + 1);
            s_cache[s_len++] = (seq_t)next_k;   /* store the term */
        }
        ++next_k;   /* move to next integer */
    }

    return s_cache[n - 1];   /* zero‑based indexing in cache */
}
