/*
 * A142404  Primes congruent to 19 mod 48.
 * https://oeis.org/A142404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 401, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 19 (mod 48) */
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"
#include "../utils/alloc_seq_array.h"

/* Static cache of primes congruent to 19 mod 48 */
static seq_t *pr19      = NULL;   /* dynamic array of found primes   */
static size_t  pr19_len = 0;      /* number of primes stored         */
static size_t  pr19_cap = 0;      /* allocated capacity              */
static seq_t  next_candidate = 0; /* next number ≡ 19 (mod 48)       */

/* Ensure the cached array has room for at least 'needed' elements. */
static void ensure_pr19_capacity(size_t needed)
{
    if (needed <= pr19_cap)
        return;

    size_t new_cap = pr19_cap ? pr19_cap * 2 : 4;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *new_arr = alloc_seq_array(new_cap);
    for (size_t i = 0; i < pr19_len; ++i)
        new_arr[i] = pr19[i];

    /* Replace the old cache; the old memory is intentionally leaked – it is
       tiny and the program lifetime is short. */
    pr19 = new_arr;
    pr19_cap = new_cap;
}

/* Return the n-th prime ≡ 19 (mod 48), 1‑based index.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined; offset is 1 */

    if (next_candidate == 0)
        next_candidate = 19; /* start with the first candidate */

    while (pr19_len < (size_t)n) {
        seq_t cand = next_candidate;
        next_candidate += 48;          /* next number of the form 19+48k */

        if (isPrimeTrial(cand)) {      /* test primality of cand */
            ensure_pr19_capacity(pr19_len + 1);
            pr19[pr19_len++] = cand;
        }
    }

    return pr19[n - 1];                /* 1‑based indexing */
}
