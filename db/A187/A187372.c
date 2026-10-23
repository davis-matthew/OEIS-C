/*
 * A187372  Numbers k such that the decimal digits of 1/k contain every digit
 *          at least once.
 * https://oeis.org/A187372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187372: numbers k such that the decimal digits of 1/k
   contain every digit 0‑9 at least once. The function below
   builds the sequence on-demand and caches the results so that
   arbitrary access to a(n) (for n ≥ 1) is fast. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Public API expected by the build system */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Helper: test whether 1/k contains all decimal digits.              */
/* ------------------------------------------------------------------ */
static bool has_all_digits(uint64_t k)
{
    if (k == (uint64_t)0) return false;

    /* Remainder after the integer part of 1/k. */
    uint64_t rem = 1 % k;
    /* Bit mask of seen digits: bit d set => digit d has appeared. */
    unsigned seen_mask = 0;
    const unsigned FULL_MASK = 0x3FFU; /* 10 bits set */

    /* Visited remainders: vector of bytes, size k (k ≤ uint64_t). */
    uint8_t *seen = (uint8_t *)calloc((size_t)k, sizeof(uint8_t));
    if (!seen) return false; /* Allocation failure: treat as false. */

    while (true)
    {
        if (seen[rem]) break;          /* cycle detected */
        seen[rem] = 1;

        /* Next decimal digit of 1/k. */
        unsigned digit = (unsigned)((rem * 10ULL) / k);
        seen_mask |= (1U << digit);
        if (seen_mask == FULL_MASK)   /* all digits seen early */
        {
            free(seen);
            return true;
        }

        rem = (rem * 10ULL) % k;
        if (rem == 0) break;           /* terminating decimal */
    }

    free(seen);
    return (seen_mask == FULL_MASK);
}

/* ------------------------------------------------------------------ */
/* Cache for computed sequence values.                                 */
/* ------------------------------------------------------------------ */
static seq_t   *cache = NULL;   /* array of a(i) values */
static size_t   cache_size = 0; /* number of values stored */
static size_t   cache_cap   = 0;/* allocated capacity */
static uint64_t next_k_to_check = 1; /* next candidate k to test */

/* Make sure the cache holds at least `need` terms. */
static void ensure_cache_size(size_t need)
{
    if (need <= cache_size) return;

    /* Grow cache capacity if necessary. */
    if (cache_cap < need)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache) { cache = new_cache; cache_cap = new_cap; }
        else /* allocation failed: keep old cache, stop adding more */
            return;
    }

    while (cache_size < need)
    {
        if (has_all_digits(next_k_to_check))
        {
            cache[cache_size++] = (seq_t)next_k_to_check;
        }
        ++next_k_to_check;
    }
}

/* ------------------------------------------------------------------ */
/* Public function returning the nth term (1‑based).                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n = 0 */
    ensure_cache_size((size_t)n);
    return cache[(size_t)(n - 1)];
}
