/*
 * A128978  a(1) = 1, a(2) = 1 and a(n) = number of terms among
 *          (a(1),a(2),...a(n-1)) which divide (a(n-1)+a(n-2)).
 * https://oeis.org/A128978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128978
   a(1) = 1, a(2) = 2
   a(n) = number of terms among (a(1),a(2),…a(n‑1)) which divide
          (a(n‑1)+a(n‑2))
   (offset = 1). This implementation memoises the sequence and
   grows the array lazily.   */

#include <stdlib.h>
#include <stddef.h>

/* Grow a dynamic array of seq_t.  The cast is needed only in C++,
   but it is harmless in C. */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Memoisation storage */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;   /* capacity of seq_cache */
static size_t seq_len = 0;   /* number of terms stored */

/* Main function required by the exercise */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1 */

    /* Ensure we have space for the term we want */
    grow_seq(&seq_cache, &seq_cap, (size_t)n);

    /* Initialise base cases if needed */
    if (seq_len == 0) {
        seq_cache[0] = (seq_t)1;  /* a(1) */
        seq_len = 1;
    }
    if (seq_len == 1) {
        seq_cache[1] = (seq_t)2;  /* a(2) */
        seq_len = 2;
    }

    /* Compute any missing terms */
    for (size_t idx = seq_len; idx < (size_t)n; ++idx) {
        seq_t sum = seq_cache[idx - 1] + seq_cache[idx - 2];
        size_t cnt = 0;
        for (size_t k = 0; k < idx; ++k) {
            if (sum % seq_cache[k] == 0) ++cnt;
        }
        seq_cache[idx] = (seq_t)cnt;
    }

    return seq_cache[(size_t)n - 1];
}
