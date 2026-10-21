/*
 * A077678  Squarefree numbers beginning with 2.
 * https://oeis.org/A077678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 6758, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer whose decimal representation starts with 2
   and which is square‑free.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* Helpers: first digit and square‑free test for 64‑bit values */
/* ------------------------------------------------------------ */
static inline int first_digit_uint64(uint64_t x)
{
    /* Returns the most significant decimal digit of x (x>0). */
    while (x >= 10) x /= 10;
    return (int)x;
}

static inline bool is_squarefree_uint64(uint64_t n)
{
    /* Brute‑force check of square–freeness (sufficient for the range
       required by this sequence). */
    if (n < 2) return true;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            /* p divides n; check if it divides n again. */
            if (n % (p * p) == 0) return false;
            /* Remove all factors of p (though we don't need them). */
            while (n % p == 0) n /= p;
        }
    }
    return true;
}

/* ------------------------------------------------------------
   Memoized sequence generator
   ------------------------------------------------------------ */
static seq_t *memo      = NULL; /* dynamically resized array of terms */
static idx_t  memo_sz   = 0;    /* number of terms stored */
static idx_t  memo_cap  = 0;    /* current allocated capacity */
static uint64_t next_test = 2;  /* next integer to test */

/* Grow the memo array if necessary. */
static void grow_memo(idx_t need_extra)
{
    if (memo_cap < memo_sz + need_extra) {
        size_t new_cap = memo_cap ? memo_cap << 1 : 8;
        while (new_cap < memo_sz + need_extra) new_cap <<= 1;
        memo = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        memo_cap = new_cap;
    }
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but harmless */

    /* Extend memoization until we have at least n terms. */
    while (memo_sz < n) {
        /* Find the next candidate starting with digit '2' that is
           square‑free. */
        while (true) {
            if (first_digit_uint64(next_test) == 2 &&
                is_squarefree_uint64(next_test)) {
                break;
            }
            ++next_test;
        }

        /* Store the candidate. */
        grow_memo(1);
        memo[memo_sz++] = (seq_t)next_test;
        ++next_test;
    }

    return memo[n - 1];
}
