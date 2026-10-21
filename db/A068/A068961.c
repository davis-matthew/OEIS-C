/*
 * A068961  Powers of 2 with exactly two 2's in their decimal digits.
 * https://oeis.org/A068961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, fini
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n) for OEIS sequence A068961: 
   "Powers of 2 with exactly two 2's in their decimal digits."
   The routine generates powers of 2 incrementally, keeps those that
   satisfy the digit condition in a cache, and serves any requested
   term from that cache.  It works for scalar (e.g. uint64_t) and
   arbitrary‑precision seq_t types.  Calls may arrive in any order.
*/

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* --------- static cache ------------------------------------------- */
static seq_t *cache = NULL;      /* array of found terms          */
static size_t cache_len = 0;     /* number of items stored        */
static size_t cache_cap = 0;     /* allocated capacity            */

/* --------- current power of two ----------------------------------- */
static idx_t last_exp = 0;     /* exponent processed so far     */
static seq_t last_val = 1;     /* 2^last_exp                    */

/* --------- helper: enlarge cache ---------------------------------- */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) {
        /* allocation failed – abort to maintain correctness */
        exit(EXIT_FAILURE);
    }
    cache = newp;
    cache_cap = new_cap;
}

/* --------- helper: count decimal '2' digits ----------------------- */
static unsigned count_twos(seq_t v)
{
    unsigned c = 0;
    while (v > 0) {
        seq_t d = v % 10;
        if (d == 2) ++c;
        v = v / 10;
    }
    return c;
}

/* --------- helper: extend cache until n terms are ready -------------*/
static void ensure_cache(size_t n)
{
    while (cache_len < n) {
        /* compute next power of 2 */
        ++last_exp;
        last_val = last_val * 2;          /* multiply by two */

        if (count_twos(last_val) == 2) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = last_val;
        }

        /* safety: stop if exponent becomes astronomically large.
           The known sequence is finite and stops at exponent 186. */
        if (last_exp > 1000000) break;
    }
}

/* --------- public function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1 */
    ensure_cache((size_t)n);
    if (n > (idx_t)cache_len) return 0;   /* term not found */
    return cache[n - 1];
}
