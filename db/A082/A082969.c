/*
 * A082969  Numbers n such that (n/4)^2-n/8 = Sum_{k=1..n} (k mod
 *          Sum_{i=0..k-1} 1-t(i)) where t(i)=A010060(i) is the Thue-Morse
 *          sequence.
 * https://oeis.org/A082969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A082969
   a(1) = 2
   for n ≥ 2: a(n) = 8 * A000069(n‑2) + 2
   where A000069(k) is the k‑th odious number (the k‑th integer with an odd number
   of 1‑bits in its binary expansion, zero based: A000069(0)=1, A000069(1)=2, …). */

#include <stdlib.h>
#include <stdint.h>

/* --- static cache for odious numbers --------------------------------- */
static idx_t     last_num      = 0;     /* last integer tested                 */
static size_t    odious_size   = 0;     /* number of cached odious numbers     */
static size_t    odious_cap    = 0;     /* capacity of the cache               */
static seq_t   *odious_cache  = NULL;   /* array of cached odious numbers      */

/* popcount for 64‑bit values (used in checksum for last_num) */
static inline unsigned
popcount_u64(uint64_t v)
{
    return __builtin_popcountll(v);
}

/* Ensure that odious_cache contains at least (need+1) elements
   (indices 0 … need).                              */
static void
ensure_odious_up_to(size_t need)
{
    /* Grow the array if necessary before adding more elements. */
    while (odious_size <= need)
    {
        /* Grow the capacity if the array is full. */
        if (odious_size == odious_cap)
        {
            size_t new_cap = odious_cap ? odious_cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(odious_cache, new_cap * sizeof(seq_t));
            if (!tmp) return;           /* allocation failure → abort */
            odious_cache = tmp;
            odious_cap = new_cap;
        }

        /* Find the next integer with an odd popcount. */
        do
        {
            ++last_num;
        } while (popcount_u64((uint64_t)last_num) % 2 == 0);

        /* Store the found odious number. */
        odious_cache[odious_size++] = (seq_t)last_num;
    }
}

/* --- public sequence function ---------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* out of range guard (never used) */

    if (n == 1) return (seq_t)2;   /* a(1) = 2 */

    /* For n ≥ 2: use the closed‑form relation. */
    size_t idx = (size_t)(n - 2);   /* zero‑based index into odious_cache   */
    ensure_odious_up_to(idx);
    return (seq_t)8 * odious_cache[idx] + (seq_t)2;
}
