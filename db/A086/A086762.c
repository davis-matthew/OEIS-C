/*
 * A086762  A piecewise recurrence relation with a(2)=7 and for n>=2: if a(n)
 *          is prime, not 31, a(n+1) = A000265(3*a(n)+1); if a(n) is odd
 *          composite, not 1, a(n+1) = A000265(a(n)+1); if a(n) is even,
 *          a(n+1) = A000265(a(n)); if a(n) is 1 or 31, find the number S(n)
 *          of occurrences of 1 and 31 among a(2),a(3),....
 * https://oeis.org/A086762
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a piecewise recurrence with a(2)=7 and for n>=2 if a(n) is
 *            prime, not 31: a(n+1)=A000265(3*a(n)+1); if a(n) is odd
 *            composite, not 1: a(n+1)=A000265(a(n)+1); if a(n) is even:
 *            a(n+1)=A000265(a(n)); if a(n) is 1 or 31: let S(n) be the number
 *            of occurrences of 1 and 31 among a(2)..a(n); use 2+S(n) in...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc, sod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_prime_big.h"
#include "../utils/sod.h"

/* cache with static state */
static seq_t *cache  = NULL;   /* cache[i] holds a(i+2)   */
static size_t cap    = 0;      /* size of allocated cache */
static size_t len    = 0;      /* number of terms stored  */
static size_t n_special = 0;   /* count of 1 or 31 up to current */

/* compute terms up to index n (a(n) for n>=2) */
static void ensure_cache(idx_t n)
{
    if (n < 2)
        return;

    /* initialise first term a(2)=7 if needed */
    if (len == 0) {
        cap = 16;
        cache = seq_alloc(cap);
        cache[0] = (seq_t)7;   /* a(2) */
        len = 1;
        n_special = 0;          /* 7 is neither 1 nor 31 */
    }

    /* append terms until we have a(n) */
    while (len < (size_t)(n - 1)) {
        seq_t cur = cache[len - 1];   /* a(current index) */
        seq_t next;

        /* current index is len+1 (since cache[0]=a(2)) */
        if (cur == (seq_t)1 || cur == (seq_t)31) {
            /* special handling */
            seq_t tmp = (seq_t)(2 + n_special);   /* S(n) = n_special */
            if (tmp == (seq_t)31) {
                next = (seq_t)47;
            } else {
                if (is_prime_big(tmp) && tmp != (seq_t)31)
                    next = sod(3 * tmp + 1);
                else if ((tmp & (seq_t)1) && tmp > (seq_t)1)
                    next = sod(tmp + 1);
                else
                    next = sod(tmp);
            }
        } else {
            /* normal recurrence */
            if (is_prime_big(cur) && cur != (seq_t)31)
                next = sod(3 * cur + 1);
            else if ((cur & (seq_t)1) && cur > (seq_t)1)
                next = sod(cur + 1);
            else
                next = sod(cur);
        }

        /* grow cache if needed */
        if (len == cap) {
            size_t new_cap = cap * 2;
            if (new_cap == 0)
                new_cap = 16;
            cache = seq_realloc(cache, cap, new_cap);
            cap = new_cap;
        }

        /* store the new term */
        cache[len] = next;

        /* update special counter if the new term is 1 or 31 */
        if (next == (seq_t)1 || next == (seq_t)31)
            ++n_special;

        ++len;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined for n < 2 */
    ensure_cache(n);
    return cache[n - 2];
}
