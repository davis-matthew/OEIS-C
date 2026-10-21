/*
 * A082461  Non-palindromic numbers whose decimal expansion is a concatenation
 *          of the form a_1 a_2 a_3 ... a_{k-1} a_k a_k a_{k-1} ... a_2 a_1 (k
 *          >= 1) or a_1 a_2 a_3 ... a_{k-1} a_k a_{k-1} ... a_2 a_1 (k >= 2)
 *          for positive integers a_1, ..., a_k. For i>1, a_i may have leading
 *          zeros.
 * https://oeis.org/A082461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th non‑palindromic decimal number that can be *
 *            split into blocks that read the same forward and backward. The
 *            code * searches for such numbers and caches the results, so
 *            subsequent calls * (in any order) are answered in O(max(n)) time
 * Range:     u8 none, u16 n <= 6394, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_decimal_palindrome, qualifies_nonpaln
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_decimal_palindrome.h"
#include "../utils/qualifies_nonpaln.h"

/* ---- digit helpers -------------------------------------------------- */

/* ---- block‑palindrome test ------------------------------------------ */

/* ---- caching -------------------------------------------------------- */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 1010;   /* first term in the sequence */
static idx_t last_found = 0;          /* number of terms cached */

/* ensure that cache has room for at least `min` elements */
static void ensure_cache_size(size_t min)
{
    if (cache_cap >= min) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min) new_cap <<= 1;
    /* cast the void* returned by realloc to seq_t* */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                  /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* ---- public function ----------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* sequence is 1‑based */

    /* produce terms until we have at least n of them */
    while (last_found < n) {
        if (qualifies_nonpaln(next_candidate)) {
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = next_candidate;
            last_found++;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
