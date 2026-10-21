/*
 * A069032  Powers of 7 with strictly increasing sum of digits.
 * https://oeis.org/A069032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069032: 7^k with strictly increasing digit sum.
   a(n) is the n-th value among the powers of 7 such that
   the sum of decimal digits strictly increases from term to term. */

/* Standard library headers. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static state shared by all calls. */
static seq_t *cache   = NULL;   /* Cached terms of the sequence. */
static idx_t  cap     = 0;      /* Allocated capacity (in terms). */
static idx_t  len     = 0;      /* Number of terms currently stored. */

/* Current power of 7 being examined. */
static seq_t cur_power = 0;   /* 7^e of last exponent examined. */
static idx_t cur_exp   = 0;   /* Current exponent e (>=0). */
static seq_t best_sum  = 0;   /* Largest digit sum seen so far. */

/* Grow the cache to accommodate at least new_cap terms. */
static void grow_cache(idx_t new_cap)
{
    size_t sz = (size_t)new_cap * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc((void *)cache, sz);
    if (!tmp) return;  /* If reallocation fails, leave cache unchanged. */
    cache = tmp;
    cap   = new_cap;
}

/* Compute the digit sum of a seq_t value. */
static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x > (seq_t)0) {
        sum += x % (seq_t)10;
        x   /= (seq_t)10;
    }
    return sum;
}

/* Ensure that at least n terms (n ≥ 1) are stored in the cache. */
static void ensure_terms(idx_t n)
{
    if (n == 0) return;   /* Undefined index, do nothing. */

    /* First call: initialise. */
    if (len == 0) {
        /* At least space for the first term. */
        if (cap < 16) grow_cache(16);

        cache[0] = (seq_t)1;      /* 7^0 = 1 */
        len      = 1;
        cur_power = (seq_t)1;     /* 7^0 */
        cur_exp   = 0;
        best_sum  = (seq_t)1;     /* Digit sum of 1. */
    }

    /* Generate terms until we have n cached values. */
    while (len < n) {
        /* Compute next power of 7. */
        ++cur_exp;
        seq_t nxt = cur_power * (seq_t)7;

        /* Detect overflow for fixed‑width builds: if nxt < cur_power
           the value has wrapped.  In that case stop generating,
           because any further values would not be representable. */
        if (nxt < cur_power) break;

        cur_power = nxt;

        seq_t s = digit_sum(cur_power);
        if (s > best_sum) {
            /* Store the new term. */
            if (len == cap)
                grow_cache(cap * 2 > 1 ? cap * 2 : 2);
            cache[len] = cur_power;
            ++len;
            best_sum = s;
        }
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[n - 1];
}
