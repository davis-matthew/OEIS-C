/*
 * A036328  Composite numbers n such that juxtaposition of prime factors of n
 *          has length 4.
 * https://oeis.org/A036328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 1048, u32 n <= 1048, u64 n <= 1048,
 *            u128 n <= 1048, big any n
 * Validated: full, pass, 1048 terms through n = 1048 (2026-11-03)
 * Helpers:   is_prime, mem, num_digits_idx, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: nth composite integer whose prime factors, concatenated in decimal,
   produce a 4‑digit string. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/num_digits_idx.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

static seq_t *seq = NULL;        /* dynamically grown array of sequence terms */
static size_t seq_len   = 0;     /* number of terms stored so far */
static size_t seq_cap   = 0;     /* capacity of seq array */
static idx_t  next_num  = 1;     /* next integer to test */

/* Return nth term (1‑based) of sequence A036328. */
seq_t a(idx_t n)
{
    while (seq_len < (size_t)n) {
        next_num++;                    /* move to next integer */
        if (is_prime(next_num))        /* skip primes */
            continue;

        /* compute the total length of the concatenated prime factors */
        uint64_t tmp = (uint64_t)next_num;
        int len = 0;
        while (tmp > 1) {
            uint64_t spf = smallest_prime_factor(tmp);
            tmp /= spf;
            len += num_digits_idx(spf);
        }

        if (len == 4) {                /* desired property */
            ensure_capacity(seq_len + 1, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));
            seq[seq_len++] = (seq_t)next_num;
        }
    }
    return seq[(size_t)n - 1];
}
